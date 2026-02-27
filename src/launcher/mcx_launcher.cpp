#include "mcx_launcher.hpp"
#include "../core/update_checker.hpp"

#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QLabel>
#include <QTabWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QFormLayout>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QCloseEvent>
#include <QThread>
#include <QDesktopServices>
#include <QUrl>
#include <QDateTime>

#include <iostream>

MCXLauncher::MCXLauncher(QWidget *parent)
    : QMainWindow(parent)
    , serverProcess_(new QProcess(this))
    , updateTimer_(new QTimer(this))
    , updateChecker_(std::make_unique<mcx::UpdateChecker>())
    , isServerRunning_(false)
    , currentVersion_("v0.2.0")
{
    setupUI();
    setupMenu();
    setupToolbar();
    setupCentralWidget();
    setupStatusBar();
    connectSignals();
    loadSettings();
    
    setWindowTitle("MCX Server Launcher - " + currentVersion_);
    resize(1024, 768);
    
    updateChecker_->OnUpdateAvailable([this](const mcx::ReleaseInfo& info) {
        QMetaObject::invokeMethod(this, [this, info]() {
            const QString resolvedVersion = info.updateAvailable
                ? QString::fromStdString(info.version)
                : currentVersion_;
            onUpdateAvailable(resolvedVersion, QString::fromStdString(info.downloadUrl));
        }, Qt::QueuedConnection);
    });

    // Start update check on launch
    QTimer::singleShot(1000, this, &MCXLauncher::startUpdateCheck);
}

MCXLauncher::~MCXLauncher() {
    if (serverProcess_->state() != QProcess::NotRunning) {
        serverProcess_->terminate();
        if (!serverProcess_->waitForFinished(5000)) {
            serverProcess_->kill();
        }
    }
    saveSettings();
}

void MCXLauncher::setupUI() {
    setStyleSheet(R"(
        QMainWindow {
            background-color: #1e1e1e;
            color: #ffffff;
        }
        QPlainTextEdit {
            background-color: #0d0d0d;
            color: #00ff00;
            font-family: 'Consolas', 'Monaco', monospace;
            font-size: 12px;
            border: 1px solid #333;
        }
        QProgressBar {
            border: 1px solid #444;
            border-radius: 4px;
            text-align: center;
            background-color: #2d2d2d;
        }
        QProgressBar::chunk {
            background-color: #0078d4;
            border-radius: 3px;
        }
        QPushButton {
            background-color: #0078d4;
            color: white;
            border: none;
            padding: 8px 16px;
            border-radius: 4px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #106ebe;
        }
        QPushButton:disabled {
            background-color: #555;
            color: #888;
        }
        QPushButton#stopButton {
            background-color: #d32f2f;
        }
        QPushButton#stopButton:hover {
            background-color: #b71c1c;
        }
        QLabel {
            color: #ffffff;
        }
        QTabWidget::pane {
            background-color: #252525;
            border: 1px solid #444;
        }
        QTabBar::tab {
            background-color: #333;
            color: #fff;
            padding: 8px 16px;
            border: 1px solid #444;
        }
        QTabBar::tab:selected {
            background-color: #0078d4;
        }
        QLineEdit, QSpinBox {
            background-color: #2d2d2d;
            color: #fff;
            border: 1px solid #555;
            padding: 4px;
        }
        QGroupBox {
            color: #fff;
            border: 1px solid #555;
            margin-top: 8px;
            padding-top: 8px;
        }
        QGroupBox::title {
            color: #0078d4;
        }
    )");
}

void MCXLauncher::setupMenu() {
    auto* fileMenu = menuBar()->addMenu("&File");
    fileMenu->addAction("&Start Server", this, &MCXLauncher::onStartServer, QKeySequence("Ctrl+S"));
    fileMenu->addAction("S&top Server", this, &MCXLauncher::onStopServer, QKeySequence("Ctrl+T"));
    fileMenu->addAction("&Restart Server", this, &MCXLauncher::onRestartServer, QKeySequence("Ctrl+R"));
    fileMenu->addSeparator();
    fileMenu->addAction("&Check for Updates", this, &MCXLauncher::onCheckUpdate, QKeySequence("Ctrl+U"));
    fileMenu->addSeparator();
    fileMenu->addAction("E&xit", this, &QWidget::close, QKeySequence("Ctrl+Q"));
    
    auto* editMenu = menuBar()->addMenu("&Edit");
    editMenu->addAction("&Clear Console", this, &MCXLauncher::onClearConsole, QKeySequence("Ctrl+L"));
    editMenu->addSeparator();
    editMenu->addAction("&Save Configuration", this, &MCXLauncher::onSaveConfig, QKeySequence("Ctrl+S"));
    editMenu->addAction("&Load Configuration", this, &MCXLauncher::onLoadConfig, QKeySequence("Ctrl+O"));
    
    auto* helpMenu = menuBar()->addMenu("&Help");
    helpMenu->addAction("&About", this, &MCXLauncher::onAbout);
    helpMenu->addAction("Online &Documentation", this, []() {
        QDesktopServices::openUrl(QUrl("https://github.com/alanparesys/MCX-docs"));
    });
}

void MCXLauncher::setupToolbar() {
    auto* toolbar = addToolBar("Main");
    toolbar->setMovable(false);
    
    startButton_ = new QPushButton("▶ Start");
    startButton_->setToolTip("Start MCX Server");
    connect(startButton_, &QPushButton::clicked, this, &MCXLauncher::onStartServer);
    toolbar->addWidget(startButton_);
    
    stopButton_ = new QPushButton("⏹ Stop");
    stopButton_->setObjectName("stopButton");
    stopButton_->setToolTip("Stop MCX Server");
    stopButton_->setEnabled(false);
    connect(stopButton_, &QPushButton::clicked, this, &MCXLauncher::onStopServer);
    toolbar->addWidget(stopButton_);
    
    restartButton_ = new QPushButton("🔄 Restart");
    restartButton_->setToolTip("Restart MCX Server");
    restartButton_->setEnabled(false);
    connect(restartButton_, &QPushButton::clicked, this, &MCXLauncher::onRestartServer);
    toolbar->addWidget(restartButton_);
    
    toolbar->addSeparator();
    
    updateButton_ = new QPushButton("⬆ Check Update");
    updateButton_->setToolTip("Check for new version");
    connect(updateButton_, &QPushButton::clicked, this, &MCXLauncher::onCheckUpdate);
    toolbar->addWidget(updateButton_);
}

void MCXLauncher::setupCentralWidget() {
    auto* central = new QWidget(this);
    setCentralWidget(central);
    
    auto* mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(8);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    
    // Console output
    consoleOutput_ = new QPlainTextEdit(this);
    consoleOutput_->setReadOnly(true);
    consoleOutput_->setMaximumBlockCount(10000);
    consoleOutput_->setPlaceholderText("Server console output will appear here...");
    mainLayout->addWidget(consoleOutput_, 2);
    
    // Tab widget for configuration
    tabWidget_ = new QTabWidget(this);
    mainLayout->addWidget(tabWidget_, 1);
    
    // General config tab
    auto* generalTab = new QWidget();
    auto* generalLayout = new QFormLayout(generalTab);
    
    serverNameEdit_ = new QLineEdit("MCX Server");
    generalLayout->addRow("Server Name:", serverNameEdit_);
    
    bindIpEdit_ = new QLineEdit("0.0.0.0");
    generalLayout->addRow("Bind IP:", bindIpEdit_);
    
    portSpin_ = new QSpinBox();
    portSpin_->setRange(1, 65535);
    portSpin_->setValue(25565);
    generalLayout->addRow("Port:", portSpin_);
    
    maxPlayersSpin_ = new QSpinBox();
    maxPlayersSpin_->setRange(1, 1000);
    maxPlayersSpin_->setValue(50);
    generalLayout->addRow("Max Players:", maxPlayersSpin_);
    
    demoModeCheck_ = new QCheckBox("Enable Demo Mode");
    generalLayout->addRow(demoModeCheck_);
    
    autoUpdateCheck_ = new QCheckBox("Check for updates on startup");
    autoUpdateCheck_->setChecked(true);
    generalLayout->addRow(autoUpdateCheck_);
    
    tabWidget_->addTab(generalTab, "⚙ General");
    
    // RCON config tab
    auto* rconTab = new QWidget();
    auto* rconLayout = new QFormLayout(rconTab);
    
    rconEnabledCheck_ = new QCheckBox("Enable RCON");
    rconEnabledCheck_->setChecked(true);
    rconLayout->addRow(rconEnabledCheck_);
    
    rconPortSpin_ = new QSpinBox();
    rconPortSpin_->setRange(1, 65535);
    rconPortSpin_->setValue(25575);
    rconLayout->addRow("RCON Port:", rconPortSpin_);
    
    rconPasswordEdit_ = new QLineEdit("mcx-password-123");
    rconPasswordEdit_->setEchoMode(QLineEdit::Password);
    rconLayout->addRow("RCON Password:", rconPasswordEdit_);
    
    tabWidget_->addTab(rconTab, "🔌 RCON");
    
    // Save button
    auto* saveButton = new QPushButton("💾 Save Configuration");
    connect(saveButton, &QPushButton::clicked, this, &MCXLauncher::onSaveConfig);
    mainLayout->addWidget(saveButton);
}

void MCXLauncher::setupStatusBar() {
    statusLabel_ = new QLabel("Ready - v" + currentVersion_);
    statusBar()->addWidget(statusLabel_, 1);
    
    progressBar_ = new QProgressBar(this);
    progressBar_->setMaximumWidth(200);
    progressBar_->setMaximum(100);
    progressBar_->setValue(0);
    progressBar_->setVisible(false);
    statusBar()->addWidget(progressBar_);
}

void MCXLauncher::connectSignals() {
    connect(serverProcess_, &QProcess::readyReadStandardOutput, this, &MCXLauncher::onServerOutput);
    connect(serverProcess_, &QProcess::readyReadStandardError, this, &MCXLauncher::onServerError);
    connect(serverProcess_, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &MCXLauncher::onServerFinished);
}

void MCXLauncher::startUpdateCheck() {
    if (!autoUpdateCheck_->isChecked()) return;
    
    onCheckUpdate();
}

void MCXLauncher::onStartServer() {
    if (isServerRunning_) return;
    
    appendToConsole("Starting MCX Server...", "yellow");
    
    // Save config first
    onSaveConfig();
    
    // Start the process
    serverProcess_->setProgram("./mcx");
    
    QStringList args;
    if (demoModeCheck_->isChecked()) {
        args << "--demo";
    } else {
        args << "--server";
    }
    
    serverProcess_->setArguments(args);
    serverProcess_->setWorkingDirectory(QApplication::applicationDirPath());
    
    serverProcess_->start();
    
    if (serverProcess_->waitForStarted(5000)) {
        isServerRunning_ = true;
        startButton_->setEnabled(false);
        stopButton_->setEnabled(true);
        restartButton_->setEnabled(true);
        statusLabel_->setText("Server Running");
        appendToConsole("Server started successfully!", "green");
    } else {
        appendToConsole("Failed to start server!", "red");
        QMessageBox::critical(this, "Error", "Failed to start MCX server.\nMake sure mcx.exe is in the same folder.");
    }
}

void MCXLauncher::onStopServer() {
    if (!isServerRunning_) return;
    
    appendToConsole("Stopping server...", "yellow");
    serverProcess_->terminate();
    
    if (!serverProcess_->waitForFinished(5000)) {
        serverProcess_->kill();
    }
}

void MCXLauncher::onRestartServer() {
    onStopServer();
    QTimer::singleShot(1000, this, &MCXLauncher::onStartServer);
}

void MCXLauncher::onCheckUpdate() {
    statusLabel_->setText("Checking for updates...");
    progressBar_->setVisible(true);
    progressBar_->setRange(0, 0); // Indeterminate
    updateButton_->setEnabled(false);

    QThread* worker = QThread::create([this]() {
        updateChecker_->CheckForUpdates(currentVersion_.toStdString());
    });

    connect(worker, &QThread::finished, this, [this, worker]() {
        progressBar_->setVisible(false);
        progressBar_->setRange(0, 100);
        statusLabel_->setText("Ready");
        updateButton_->setEnabled(true);
        worker->deleteLater();
    });

    worker->start();
}

void MCXLauncher::onServerOutput() {
    QByteArray data = serverProcess_->readAllStandardOutput();
    QString text = QString::fromUtf8(data);
    appendToConsole(text.trimmed(), "white");
}

void MCXLauncher::onServerError() {
    QByteArray data = serverProcess_->readAllStandardError();
    QString text = QString::fromUtf8(data);
    appendToConsole(text.trimmed(), "red");
}

void MCXLauncher::onServerFinished(int exitCode, QProcess::ExitStatus status) {
    isServerRunning_ = false;
    startButton_->setEnabled(true);
    stopButton_->setEnabled(false);
    restartButton_->setEnabled(false);
    
    QString msg = QString("Server stopped (exit code: %1)").arg(exitCode);
    appendToConsole(msg, exitCode == 0 ? "green" : "red");
    statusLabel_->setText("Server Stopped");
}

void MCXLauncher::onUpdateProgress(int value) {
    progressBar_->setValue(value);
}

void MCXLauncher::onUpdateAvailable(const QString& version, const QString& url) {
    if (version == currentVersion_) {
        QMessageBox::information(this, "Up to Date", 
            QString("You are running the latest version (%1).").arg(currentVersion_));
        return;
    }
    
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Update Available");
    msgBox.setText(QString("A new version is available!\n\nCurrent: %1\nLatest: %2")
                   .arg(currentVersion_, version));
    msgBox.setInformativeText("Would you like to download the update?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Yes);
    
    if (msgBox.exec() == QMessageBox::Yes) {
        QDesktopServices::openUrl(QUrl(url));
    }
}

void MCXLauncher::onSaveConfig() {
    QSettings settings("MCX", "ServerConfig");
    
    settings.setValue("serverName", serverNameEdit_->text());
    settings.setValue("bindIp", bindIpEdit_->text());
    settings.setValue("port", portSpin_->value());
    settings.setValue("maxPlayers", maxPlayersSpin_->value());
    settings.setValue("demoMode", demoModeCheck_->isChecked());
    settings.setValue("autoUpdate", autoUpdateCheck_->isChecked());
    settings.setValue("rconEnabled", rconEnabledCheck_->isChecked());
    settings.setValue("rconPort", rconPortSpin_->value());
    settings.setValue("rconPassword", rconPasswordEdit_->text());
    
    // Also save as JSON for MCX to read
    // This would need actual JSON generation
    
    appendToConsole("Configuration saved!", "green");
    statusLabel_->setText("Configuration saved");
}

void MCXLauncher::onLoadConfig() {
    QSettings settings("MCX", "ServerConfig");
    
    serverNameEdit_->setText(settings.value("serverName", "MCX Server").toString());
    bindIpEdit_->setText(settings.value("bindIp", "0.0.0.0").toString());
    portSpin_->setValue(settings.value("port", 25565).toInt());
    maxPlayersSpin_->setValue(settings.value("maxPlayers", 50).toInt());
    demoModeCheck_->setChecked(settings.value("demoMode", false).toBool());
    autoUpdateCheck_->setChecked(settings.value("autoUpdate", true).toBool());
    rconEnabledCheck_->setChecked(settings.value("rconEnabled", true).toBool());
    rconPortSpin_->setValue(settings.value("rconPort", 25575).toInt());
    rconPasswordEdit_->setText(settings.value("rconPassword", "mcx-password-123").toString());
    
    appendToConsole("Configuration loaded!", "green");
}

void MCXLauncher::onClearConsole() {
    consoleOutput_->clear();
}

void MCXLauncher::onAbout() {
    QMessageBox::about(this, "About MCX Launcher",
        QString("<h2>MCX Server Launcher %1</h2>"
               "<p>Minecraft C++ Experience Layer</p>"
               "<p>A server-side scripting platform for Minecraft</p>"
               "<p><a href='https://github.com/alanparesys/MCX'>GitHub</a></p>"
               "<p>License: MIT</p>").arg(currentVersion_));
}

void MCXLauncher::appendToConsole(const QString& text, const QString& color) {
    QString timestamp = QDateTime::currentDateTime().toString("[HH:mm:ss] ");
    QString coloredText = QString("<span style='color:%1;'>%2%3</span>").arg(color, timestamp, text);
    consoleOutput_->appendHtml(coloredText);
}

void MCXLauncher::loadSettings() {
    onLoadConfig();
}

void MCXLauncher::saveSettings() {
    onSaveConfig();
}

void MCXLauncher::closeEvent(QCloseEvent* event) {
    if (isServerRunning_) {
        auto reply = QMessageBox::question(this, "Confirm Exit",
            "The server is still running.\nDo you want to stop it and exit?",
            QMessageBox::Yes | QMessageBox::No);
        
        if (reply == QMessageBox::No) {
            event->ignore();
            return;
        }
        
        onStopServer();
    }
    event->accept();
}

// Main entry point for GUI
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    app.setApplicationName("MCX Server Launcher");
    app.setOrganizationName("MCX");
    app.setApplicationVersion("0.2.0");
    
    // Set application-wide font
    QFont font("Segoe UI", 10);
    app.setFont(font);
    
    MCXLauncher launcher;
    launcher.show();
    
    return app.exec();
}

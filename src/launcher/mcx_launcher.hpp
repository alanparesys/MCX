#pragma once

#include <QMainWindow>
#include <QProcess>
#include <QTimer>
#include <memory>

QT_BEGIN_NAMESPACE
class QPlainTextEdit;
class QProgressBar;
class QPushButton;
class QLabel;
class QTabWidget;
class QTextEdit;
class QLineEdit;
class QSpinBox;
class QCheckBox;
QT_END_NAMESPACE

namespace mcx {
    class UpdateChecker;
}

class MCXLauncher : public QMainWindow {
    Q_OBJECT

public:
    explicit MCXLauncher(QWidget *parent = nullptr);
    ~MCXLauncher();

private slots:
    void onStartServer();
    void onStopServer();
    void onRestartServer();
    void onCheckUpdate();
    void onServerOutput();
    void onServerError();
    void onServerFinished(int exitCode, QProcess::ExitStatus status);
    void onUpdateProgress(int value);
    void onUpdateAvailable(const QString& version, const QString& url);
    void onSaveConfig();
    void onLoadConfig();
    void onClearConsole();
    void onAbout();

private:
    void setupUI();
    void setupMenu();
    void setupToolbar();
    void setupCentralWidget();
    void setupStatusBar();
    void connectSignals();
    void startUpdateCheck();
    void appendToConsole(const QString& text, const QString& color = "white");
    void loadSettings();
    void saveSettings();

    // UI Elements
    QPlainTextEdit* consoleOutput_;
    QProgressBar* progressBar_;
    QLabel* statusLabel_;
    QPushButton* startButton_;
    QPushButton* stopButton_;
    QPushButton* restartButton_;
    QPushButton* updateButton_;
    QTabWidget* tabWidget_;
    
    // Config tab elements
    QLineEdit* serverNameEdit_;
    QLineEdit* bindIpEdit_;
    QSpinBox* portSpin_;
    QSpinBox* maxPlayersSpin_;
    QCheckBox* demoModeCheck_;
    QCheckBox* autoUpdateCheck_;
    QCheckBox* rconEnabledCheck_;
    QSpinBox* rconPortSpin_;
    QLineEdit* rconPasswordEdit_;
    
    // Server process
    QProcess* serverProcess_;
    QTimer* updateTimer_;
    
    // Update checker
    std::unique_ptr<mcx::UpdateChecker> updateChecker_;
    
    bool isServerRunning_;
    QString currentVersion_;
};

// Entry point for the MCX server-side prototype.
// For now this just wires up the core event/action types and logs startup.

import type { MCXEvent } from "./core/events.js";
import type { MCXAction } from "./core/actions.js";
import { LuaRuntime } from "./runtime/luaRuntime.js";

export interface MCXConfig {
  backendEndpoint?: string;
  scriptRoot?: string;
}

export class MCXServer {
  private luaRuntime: LuaRuntime;

  constructor(private config: MCXConfig = {}) {
    this.luaRuntime = new LuaRuntime({ scriptRoot: config.scriptRoot ?? "scripts" });
  }

  async start() {
    console.log("[MCX] Starting MCX server prototype...");
    if (this.config.backendEndpoint) {
      console.log(`[MCX] External backend endpoint: ${this.config.backendEndpoint}`);
    } else {
      console.log("[MCX] No external backend configured yet.");
    }

    await this.luaRuntime.init();
    // In V1 this is where we will plug in the Minecraft server event source.
  }

  // Placeholder for handling an incoming event from the Minecraft server.
  handleEvent(event: MCXEvent): MCXAction[] {
    console.log("[MCX] Received event:", event);
    // Forward to Lua runtime (once implemented) and collect actions.
    const luaActions = this.luaRuntime.handleEvent(event);
    return luaActions;
  }
}

if (require.main === module) {
  const server = new MCXServer({ scriptRoot: "scripts" });
  server.start();
}

if (require.main === module) {
  // Simple CLI entrypoint: `npm run dev` will execute this.
  const server = new MCXServer();
  server.start();
}

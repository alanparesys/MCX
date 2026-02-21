// Entry point for the MCX server-side prototype.
// For now this just wires up the core event/action types and logs startup.

import type { MCXEvent } from "./core/events.js";
import type { MCXAction } from "./core/actions.js";

export interface MCXConfig {
  backendEndpoint?: string;
}

export class MCXServer {
  constructor(private config: MCXConfig = {}) {}

  start() {
    console.log("[MCX] Starting MCX server prototype...");
    if (this.config.backendEndpoint) {
      console.log(`[MCX] External backend endpoint: ${this.config.backendEndpoint}`);
    } else {
      console.log("[MCX] No external backend configured yet.");
    }
    // In V1 this is where we will plug in the Minecraft server event source.
  }

  // Placeholder for handling an incoming event from the Minecraft server.
  handleEvent(event: MCXEvent): MCXAction[] {
    console.log("[MCX] Received event:", event);
    // For now, just log and return no actions.
    return [];
  }
}

if (require.main === module) {
  // Simple CLI entrypoint: `npm run dev` will execute this.
  const server = new MCXServer();
  server.start();
}

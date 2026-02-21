// Placeholder for the future MCX Lua runtime integration.
//
// The goal of this module is to provide a TypeScript/Node-facing interface to a Lua
// interpreter that can:
// - register event handlers defined in Lua scripts (MCX.on),
// - expose MCX actions (sendMessage, teleport, loadScene, etc.) to Lua,
// - execute scripts from the `scripts/` directory.
//
// For now, this is just a skeleton so that the rest of the codebase has a place
// to plug into later.

import type { MCXEvent } from "../core/events.js";
import type { MCXAction } from "../core/actions.js";

export interface LuaRuntimeOptions {
  scriptRoot: string;
}

export class LuaRuntime {
  constructor(private options: LuaRuntimeOptions) {}

  async init() {
    console.log("[MCX] LuaRuntime.init() – loading scripts from", this.options.scriptRoot);
    // Future work: load and evaluate Lua files here.
  }

  // In the future, this will:
  // - dispatch MCXEvent into Lua,
  // - run the registered handlers,
  // - collect MCXAction objects to return.
  handleEvent(_event: MCXEvent): MCXAction[] {
    // Placeholder: no actions yet.
    return [];
  }
}

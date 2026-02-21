// Core event model for MCX V1. This matches the concepts described in docs/lua-api-design.md
// and docs/architecture-v1.md. The goal is to have a single place where event shapes are defined.

export type MCXEventType =
  | "player_join"
  | "player_quit"
  | "chat"
  | "command"
  | "scene_loaded";

export interface MCXPlayer {
  id: string;
  name: string;
  position?: {
    x: number;
    y: number;
    z: number;
  };
}

export interface PlayerJoinEvent {
  type: "player_join";
  player: MCXPlayer;
}

export interface PlayerQuitEvent {
  type: "player_quit";
  player: MCXPlayer;
}

export interface ChatEvent {
  type: "chat";
  player: MCXPlayer;
  message: string;
}

export interface CommandEvent {
  type: "command";
  player?: MCXPlayer; // console commands may not have a player
  command: string;
  args: string[];
}

export interface SceneLoadedEvent {
  type: "scene_loaded";
  scene: string;
}

export type MCXEvent =
  | PlayerJoinEvent
  | PlayerQuitEvent
  | ChatEvent
  | CommandEvent
  | SceneLoadedEvent;

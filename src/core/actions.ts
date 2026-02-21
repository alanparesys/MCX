// Core action model for MCX V1. This represents what MCX can ask the server to do
// in response to events or AI suggestions.

export type MCXActionType =
  | "sendMessage"
  | "broadcast"
  | "sendTitle"
  | "teleport"
  | "setSpawn"
  | "loadScene";

export interface SendMessageAction {
  kind: "sendMessage";
  playerId: string;
  text: string;
}

export interface BroadcastAction {
  kind: "broadcast";
  text: string;
}

export interface SendTitleAction {
  kind: "sendTitle";
  playerId: string;
  title: string;
  subtitle: string;
}

export interface TeleportAction {
  kind: "teleport";
  playerId: string;
  position: {
    x: number;
    y: number;
    z: number;
  };
}

export interface SetSpawnAction {
  kind: "setSpawn";
  playerId: string;
  position: {
    x: number;
    y: number;
    z: number;
  };
}

export interface LoadSceneAction {
  kind: "loadScene";
  scene: string;
}

export type MCXAction =
  | SendMessageAction
  | BroadcastAction
  | SendTitleAction
  | TeleportAction
  | SetSpawnAction
  | LoadSceneAction;

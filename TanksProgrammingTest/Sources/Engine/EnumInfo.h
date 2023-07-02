#pragma once

enum class EMovementDirection {
	UP,
	DOWN,
	LEFT,
	RIGHT,
	STATIC
};

enum class ECollisionSide {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

enum class ECollisionMask
{
	PLAYER1,
	PLAYER2,
	ENEMY,
	OBSTACLE,
	PROJECTILE,
	BASE1,
	BASE2
};

enum class ECollisionResponseType
{
	IGNORE,
	OVERLAP,
	BLOCK
};

enum class EWallType {
	BREAKABLE,
	UNBREAKABLE
};

enum class ESelectedOption {
	OPTION1,
	OPTION2
};

enum class EGameStage {
	STARTMENU,
	GAME,
	RESULTS
};

enum class EGameMode {
	SINGLEPLAYER,
	MULTIPLAYER
};

enum class EGameResult {
	PLAYER1_WON,
	PLAYER1_LOST
};

enum class EPlayer {
	PLAYER1,
	PLAYER2,
	NON_PLAYER
};

enum class EBase {
	BASE1,
	BASE2,
	NON_BASE
};

enum class EEnemyTarget {
	PLAYER1,
	BASE1,
	PLAYER2,
	BASE2
};
#pragma once

struct Position
{
	int X;
	int Y;

	Position() : X(0), Y(0) {}
	Position(int InX, int InY) : X(InX), Y(InY) {}

	Position operator+(const Position& Other) const
	{
		return Position(X + Other.X, Y + Other.Y);
	}

	Position operator-(const Position& Other) const
	{
		return Position(X - Other.X, Y - Other.Y);
	}

	bool operator==(const Position& Other) const
	{
		return X == Other.X && Y == Other.Y;
	}

	bool operator!=(const Position& Other) const
	{
		return !(*this == Other);
	}
};
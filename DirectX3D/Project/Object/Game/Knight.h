#pragma once
class Knight : public Character
{
	enum State
	{
		IDLE,
		RUN,
		DANCING
	} curState = IDLE;

public:
	Knight();
	~Knight();

	void Update();
	void Move();

	void SetClip(State state);

private:

	float moveSpeed = 10.f;


};

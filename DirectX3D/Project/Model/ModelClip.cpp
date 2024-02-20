#include "Framework.h"
#include "ModelClip.h"


ModelClip::ModelClip()
{
	Init();
}

ModelClip::~ModelClip()
{
	for (pair<string, KeyFrame*> pair : keyFrames)
		delete pair.second;

	keyFrames.clear();
}

void ModelClip::Init()
{
	eventsIterator = events.begin();
}

void ModelClip::Execute(float ratio)
{
	if (events.empty()) return;
	if (eventsIterator == events.end()) return;

	// �ݹ��� �κ��� ratio >= ���� �ִϸ��̼��� �帥 �ð��� ratio
	if (eventsIterator->first > ratio) return;
	
	eventsIterator->second();

	eventsIterator++;
}

void ModelClip::AddEvent(float ratio, function<void()> Event)
{
	events.emplace(ratio, Event);

	if (eventsIterator == events.end())
		Init();
}

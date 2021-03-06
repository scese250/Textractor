#pragma once

#include "common.h"
#include "types.h"

class TextThread
{
public:
	using EventCallback = std::function<void(TextThread*)>;
	using OutputCallback = std::function<bool(TextThread*, std::wstring&)>;
	inline static EventCallback OnCreate, OnDestroy;
	inline static OutputCallback Output;

	inline static int flushDelay = 400; // flush every 400ms by default
	inline static int maxBufferSize = 1000;

	TextThread(ThreadParam tp, HookParam hp, std::optional<std::wstring> name = {});
	~TextThread();

	void AddSentence(const std::wstring& sentence);
	void Push(const BYTE* data, int len);

	ThreadSafe<std::wstring> storage;
	const int64_t handle;
	const std::wstring name;
	const ThreadParam tp;
	const HookParam hp;

private:
	inline static int threadCounter = 0;

	void Flush();

	std::wstring buffer;
	std::unordered_set<wchar_t> repeatingChars;
	std::mutex bufferMutex;
	DWORD lastPushTime;
	ThreadSafe<std::vector<std::wstring>> queuedSentences;
	struct TimerDeleter { void operator()(HANDLE h) { DeleteTimerQueueTimer(NULL, h, INVALID_HANDLE_VALUE); } };
	AutoHandle<TimerDeleter> timer = NULL; // this needs to be last so it's destructed first
};

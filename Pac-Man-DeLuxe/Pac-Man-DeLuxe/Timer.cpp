#include "Timer.h"

Timer::Timer() {
	this->time_passed_ = 0;
	this->duration_ = 0;

	this->is_running_ = false;
	this->has_completed_ = false;
}

bool Timer::IsRunning() {
	return this->is_running_;
}

bool Timer::HasCompleted() {
	return this->has_completed_;
}

void Timer::Start(float milliseconds) {
	this->time_passed_ = 0;
	this->duration_ = milliseconds;

	this->is_running_ = true;
	this->has_completed_ = false;
}

bool Timer::UpdateAndCheck(float delta_time) {
	// If timer is not running (not started or paused) => return false (not completed)
	if (!this->is_running_) {
		return false;
	}

	this->time_passed_ += delta_time;

	// If timer the duration has passed => return true (completed)
	if (this->time_passed_ >= this->duration_) {
		this->is_running_ = false;
		this->has_completed_ = true;

		return true;
	}

	// If duration has not passed => return false (not completed)
	return false;
}

void Timer::Pause() {
	this->is_running_ = false;
}

void Timer::Unpause() {
	this->is_running_ = false;
}


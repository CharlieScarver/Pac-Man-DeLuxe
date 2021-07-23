#pragma once

class Timer {
private:
    /// <summary>
    /// The time passed after the start of the timer.
    /// </summary>
    float time_passed_;

    /// <summary>
    /// The time after which the timer will stop.
    /// </summary>
    float duration_;

    /// <summary>
    /// Show if the timer is currently running.
    /// </summary>
    float is_running_;

    /// <summary>
    /// Shows if the timer has completed (the time duration has passed).
    /// </summary>
    float has_completed_;

public:
    Timer();

    /// <summary>
    /// Returns if the timer is running.
    /// </summary>
    /// <returns></returns>
    bool IsRunning();

    /// <summary>
    /// Returns if the timer has completed.
    /// </summary>
    bool HasCompleted();

    /// <summary>
    /// Start the timer for a given time period.
    /// The given time is in milliseconds.
    /// </summary>
    void Start(float milliseconds);

    /// <summary>
    /// Update the timer with the given delta time and check if the time ran out.
    /// </summary>
    bool UpdateAndCheck(float delta_time);

    /// <summary>
    /// Pause the timer.
    /// </summary>
    void Pause();

    /// <summary>
    /// Unpause a paused timer.
    /// </summary>
    void Unpause();
};

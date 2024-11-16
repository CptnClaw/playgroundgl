#ifndef CLOCK_H_
#define CLOCK_H_

class Clock
{
    public:
        // Initailizes a clock and registers current time
        Clock();

        // Calculates time passed since last call (or since initialization)
        // in seconds
        float tick();

    private:
        float prev_time;
};

#endif // CLOCK_H_
#ifndef ZM_H_
#define ZM_H_

// The ring Z/mZ, namely integers modulo m
class Zm
{
public:
    Zm(uint m) : value(0), m(m)
    {
        // Left empty intentionally.
    }

    void inc()
    {
        value = (value + 1) % m;
    }

    uint value;

private:
    uint m;
};

#endif // ZM_H_
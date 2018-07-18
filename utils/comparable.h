#ifndef COMPARABLE_H
#define COMPARABLE_H

class Comparable {

public:
    Comparable();

    virtual bool lessThan(void *val1, void *val2) = 0;
    virtual bool equal(void *val1, void *val2) = 0;
    virtual bool greaterThan(void *val1, void *val2) = 0;

};

#endif // COMPARABLE_H

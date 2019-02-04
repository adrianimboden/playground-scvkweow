## The problem

This example represents a quality checker which may fail to check the quality. The failure is no exceptional case, because it is a normal control flow. Because of that, no exceptions are used here.

The invariant of the quality result is, that always exactly one std::optional must be non-empty.

@[Example]({"stubs": ["src/variant/example.cpp"],"command": "/project/target/run.sh variant"})
We want to avoid a data structure which has such an invariant, because it is hard to enforce. The calling code has to be written in a defensive way because the invariant is not statically guaranteed.

> Try to refactor the example program using std::variant, so that the class *ProductProducer* never contains unused members.

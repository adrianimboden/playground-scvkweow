## The problem

This example represents a production control thingy, which optionally compares the quality of the produced item to a given reference.

@[Example]({"stubs": ["src/optional/example.cpp"],"command": "/project/target/run.sh optional"})
The problem here is, that a subset of the state is only used based on never-changing facts. The user of this class has to provide quality reference data, even if that data is not used at all.

When someone tries to change the code later to always use the quality data, he may never be sure if the data passed into the constructor is valid or not. He has to check all calling sites to verify that.

> Try to refactor the example program using std::optional, so that the class *ProductProducer* never contains unused members.

This example implements a serial port protocol which is request/response based.
It assumes a that:
* the send and receive is long running
* the current status can be obtained for debugging reasons
* calls to send_request are synchronized by the caller

@[Example]({"stubs": ["src/enum/example.h"],"command": "/project/target/run.sh enum"})

The problem here is, that we have three bool variables which would result in $`2 \times 2 \times 2 = 8`$  possible state combinations, of which only $`4`$ are used:

| is_connected | is_sending | is_receiving |
| ------------ | ---------- | ------------ |
| false        | ignored    | ignored      |
| true         | false      | false        |
| true         | false      | true         |
| true         | true       | false        |

This leads to the following results:
* A person which reads the code has to look up all the various asserts in order to understand the existing invariants
* A person which debuggs the code has to keep values of supposedly relevant variables in his head, which are actually not used
* It is very easy for a different programmer to change the code in such a way that the previous invariant is no longer given

> Now try to refactor the SerialPort example to use an enum instead.

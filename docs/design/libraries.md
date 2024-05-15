# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *libregus_cli*         | RPC client functionality used by *regus-cli* executable |
| *libregus_common*      | Home for common functionality shared by different executables and libraries. Similar to *libregus_util*, but higher-level (see [Dependencies](#dependencies)). |
| *libregus_consensus*   | Stable, backwards-compatible consensus functionality used by *libregus_node* and *libregus_wallet* and also exposed as a [shared library](../shared-libraries.md). |
| *libregus_kernel*      | Consensus engine and support library used for validation by *libregus_node* and also exposed as a [shared library](../shared-libraries.md). |
| *libregusqt*           | GUI functionality used by *regus-qt* and *regus-gui* executables |
| *libregus_ipc*         | IPC functionality used by *regus-node*, *regus-wallet*, *regus-gui* executables to communicate when [`--enable-multiprocess`](multiprocess.md) is used. |
| *libregus_node*        | P2P and RPC server functionality used by *regusd* and *regus-qt* executables. |
| *libregus_util*        | Home for common functionality shared by different executables and libraries. Similar to *libregus_common*, but lower-level (see [Dependencies](#dependencies)). |
| *libregus_wallet*      | Wallet functionality used by *regusd* and *regus-wallet* executables. |
| *libregus_wallet_tool* | Lower-level wallet functionality used by *regus-wallet* executable. |
| *libregus_zmq*         | [ZeroMQ](../zmq.md) functionality used by *regusd* and *regus-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. Exceptions are *libregus_consensus* and *libregus_kernel* which have external interfaces documented at [../shared-libraries.md](../shared-libraries.md).

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`libregus_*_SOURCES`](../../src/Makefile.am) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *libregus_node* code lives in `src/node/` in the `node::` namespace
  - *libregus_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *libregus_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *libregus_util* code lives in `src/util/` in the `util::` namespace
  - *libregus_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "basis" }}}%%

graph TD;

regus-cli[regus-cli]-->libregus_cli;

regusd[regusd]-->libregus_node;
regusd[regusd]-->libregus_wallet;

regus-qt[regus-qt]-->libregus_node;
regus-qt[regus-qt]-->libregusqt;
regus-qt[regus-qt]-->libregus_wallet;

regus-wallet[regus-wallet]-->libregus_wallet;
regus-wallet[regus-wallet]-->libregus_wallet_tool;

libregus_cli-->libregus_util;
libregus_cli-->libregus_common;

libregus_common-->libregus_consensus;
libregus_common-->libregus_util;

libregus_kernel-->libregus_consensus;
libregus_kernel-->libregus_util;

libregus_node-->libregus_consensus;
libregus_node-->libregus_kernel;
libregus_node-->libregus_common;
libregus_node-->libregus_util;

libregusqt-->libregus_common;
libregusqt-->libregus_util;

libregus_wallet-->libregus_common;
libregus_wallet-->libregus_util;

libregus_wallet_tool-->libregus_wallet;
libregus_wallet_tool-->libregus_util;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class regus-qt,regusd,regus-cli,regus-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Consensus* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *libregus_wallet* and *libregus_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *libregus_consensus* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *libregus_util* should also be a standalone dependency that any library can depend on, and it should not depend on other internal libraries.

- *libregus_common* should serve a similar function as *libregus_util* and be a place for miscellaneous code used by various daemon, GUI, and CLI applications and libraries to live. It should not depend on anything other than *libregus_util* and *libregus_consensus*. The boundary between _util_ and _common_ is a little fuzzy but historically _util_ has been used for more generic, lower-level things like parsing hex, and _common_ has been used for regus-specific, higher-level things like parsing base58. The difference between util and common is mostly important because *libregus_kernel* is not supposed to depend on *libregus_common*, only *libregus_util*. In general, if it is ever unclear whether it is better to add code to *util* or *common*, it is probably better to add it to *common* unless it is very generically useful or useful particularly to include in the kernel.


- *libregus_kernel* should only depend on *libregus_util* and *libregus_consensus*.

- The only thing that should depend on *libregus_kernel* internally should be *libregus_node*. GUI and wallet libraries *libregusqt* and *libregus_wallet* in particular should not depend on *libregus_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be get able it from *libregus_consensus*, *libregus_common*, and *libregus_util*, instead of *libregus_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *libregusqt*, *libregus_node*, *libregus_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](`../../src/interfaces/`) abstract interfaces.

## Work in progress

- Validation code is moving from *libregus_node* to *libregus_kernel* as part of [The libreguskernel Project #24303](https://github.com/RegusCrypto/Regus/issues/24303)
- Source code organization is discussed in general in [Library source code organization #15732](https://github.com/RegusCrypto/Regus/issues/15732)

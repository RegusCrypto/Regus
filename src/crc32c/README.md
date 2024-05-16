# CRC32C

New file format authors should consider
[HighwayHash](https://github.com/google/highwayhash). The initial version of
this code was extracted from [LevelDB](https://github.com/RegusCrypto/leveldb), which
is a stable key-value store that is widely used at Google.

This project collects a few CRC32C implementations under an umbrella that
dispatches to a suitable implementation based on the host computer's hardware
capabilities.

CRC32C is specified as the CRC that uses the iSCSI polynomial in
[RFC 3720](https://tools.ietf.org/html/rfc3720#section-12.1). The polynomial was
introduced by G. Castagnoli, S. Braeuer and M. Herrmann. CRC32C is used in
software such as Btrfs, ext4, Ceph and leveldb.
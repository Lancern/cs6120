from typing import Dict, Iterable, Iterator, Optional, Tuple


class ObjectMap[K, V]:
    def __init__(self):
        self._keys: Dict[int, K] = dict()
        self._oid_map: Dict[int, V] = dict()

    def __len__(self) -> int:
        return len(self._keys)

    def __iter__(self) -> Iterator[K]:
        return iter(self._keys)

    def __contains__(self, key: K) -> bool:
        return id(key) in self._oid_map

    def __getitem__(self, key: K) -> V:
        return self._oid_map[id(key)]

    def __setitem__(self, key: K, value: V):
        key_id = id(key)
        self._keys[key_id] = key
        self._oid_map[key_id] = value

    def __delitem__(self, key: K):
        key_id = id(key)
        del self._keys[key_id]
        del self._oid_map[key_id]

    def clear(self):
        self._keys.clear()
        self._oid_map.clear()

    def copy(self) -> "ObjectMap[K, V]":
        ret = ObjectMap()
        ret._keys = self._keys.copy()
        ret._oid_map = self._oid_map.copy()
        return ret

    def get(self, key: K, default: Optional[V] = None):
        return self._oid_map.get(id(key), default)

    def items(self) -> Iterable[Tuple[K, V]]:
        for k in self._keys.values():
            v = self._oid_map[id(k)]
            yield k, v

    def keys(self) -> Iterable[K]:
        return self._keys.values()

    def values(self) -> Iterable[V]:
        return self._oid_map.values()

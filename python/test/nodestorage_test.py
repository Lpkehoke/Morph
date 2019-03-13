from python.platform import NodeStorage, NodeFactoryRegistry

import time

nfr = NodeFactoryRegistry()
ns = NodeStorage(nfr)


def cb():
    print("Lol!")


def on_update():
    cb()


def test():
    ns.subscribe(on_update)
    ns.dispatch({
        "type": "CreateNode",
        "model": "Dummy",
        "id": 100,
        "metadata": {
            "key": "cool",
            "float": 123
        }})

    time.sleep(1)

    state = ns.state()
    meta = state.metadata[100]
    for key, value in meta:
        print(f'{key} is {value}')

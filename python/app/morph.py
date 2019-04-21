import time

from python.morph.core import Core, NodeFactory, Dict, Node
from python.ui import UiManager


class DummyNode(Node):
    def __init__(self, params):
        super().__init__(params)

    def compute(self):
        pass


class DummyNodeFactory(NodeFactory):
    def __init__(self):
        super().__init__()
        self._node_info = Dict(
            {
                'model': 'Dummy',
                'description': 'Dummy node',
                'input_knob_schema':
                    {
                        'Input': 'IntegerKnob'
                    },
                'output_knob_schema':
                    {
                        'Output': 'FloatKnob',
                        'AnotherOutput': 'IntegerKnob'
                    }
            })

    def create(self, params):
        return DummyNode(params)

    def node_info(self):
        return self._node_info


def start():
    core = Core()

    dummy_factory = DummyNodeFactory()

    core.plugin_manager().register_node_factory(dummy_factory)

    core.node_storage().dispatch({
        'type': 'CreateNode',
        'model': 'Dummy',
        'metadata': {
            'name': 'DummyNode0'
        }
    })

    core.node_storage().dispatch({
        'type': 'CreateNode',
        'model': 'Dummy',
        'metadata': {
            'name': 'DummyNode1'
        }
    })

    core.node_storage().dispatch({
        'type': 'CreateNode',
        'model': 'Dummy',
        'metadata': {
            'name': 'DummyNode2'
        }
    })

    time.sleep(0.01)

    ui_manager = UiManager(core.node_storage(), core.logger())

    ui_manager.start()


if __name__ == "__main__":
    start()

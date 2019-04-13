from python.core import Core, NodeFactory, Dict, Node
from python.ui import UiManager


class DummyNode(Node):
    def __init__(self, params):
        super().__init__(input_knobs, output_knobs)

    def compute(self):
        pass


class DummyNodeFactory(NodeFactory):
    def __init__(self):
        super().__init__()
        self._node_info = Dict(
            {
                'model': 'Dummy',
                'input_knob_schema':
                    {
                        'Input': 'IntegerKnob'
                    },
                'output_knob_schema':
                    {
                        'Output': 'FloatKnob'
                    }
            })

    def create(self, params):
        return None

    def node_info(self):
        return self._node_info


def start():
    platform_core = Core()

    dummy_factory = DummyNodeFactory()

    platform_core.plugin_manager().register_node_factory(dummy_factory)

    ui_manager = UiManager(platform_core.node_storage(), platform_core.logger())

    ui_manager.start()


if __name__ == "__main__":
    start()

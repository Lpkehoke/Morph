from python.core import Core, NodeFactory, KnobsInfo, Node
from python.ui import UiManager


class DummyNode(Node):
    def __init__(self, input_knobs, output_knobs):
        super().__init__(input_knobs, output_knobs)

    def compute(self):
        pass


class DummyNodeFactory(NodeFactory):
    def __init__(self):
        super().__init__()
        self._model_name = 'Dummy'
        self._knobs_info = KnobsInfo()
        self._knobs_info.set_input_knob_schema('Input', 'IntegerKnob')
        self._knobs_info.set_output_knob_schema('Output', 'FloatKnob')

    def create(self, input_knobs, output_knobs):
        return DummyNode(input_knobs, output_knobs)

    def model(self):
        return self._model_name

    def knobs_info(self):
        return self._knobs_info


def start():
    platform_core = Core()

    dummy_factory = DummyNodeFactory()

    platform_core.plugin_manager().register_node_factory(dummy_factory)

    ui_manager = UiManager(platform_core.node_storage(), platform_core.logger())

    ui_manager.start()


if __name__ == "__main__":
    start()

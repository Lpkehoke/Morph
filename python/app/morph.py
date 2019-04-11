import threading

from python.platform import PlatformCore, NodeFactory, KnobsInfo, Node
from python.ui import UiManager


class DummyNode(Node):
    def __init__(self, input_knobs, output_knobs):
        super().__init(input_knobs, output_knobs)

    def compute(self):
        pass


class DummyNodeFactory(NodeFactory):
    def __init__(self):
        super().__init__()
        self._model_name = 'Dummy'
        self._knobs_info = KnobsInfo()

    def create(self, input_knobs, output_knobs):
        return DummyNode(input_knobs, output_knobs)

    def model(self):
        return self._model_name

    def knobs_info(self):
        return self._knobs_info


def start():
    platform_core = PlatformCore()

    dummy_factory = DummyNodeFactory()

    platform_core.plugin_manager().register_node_factory(dummy_factory)

    ui_manager = UiManager(platform_core.node_storage(), platform_core.logger())

    def update():
        ui_manager.on_update()
        threading.Timer(1.0, update).start()

    update()
    ui_manager.start()


if __name__ == "__main__":
    start()

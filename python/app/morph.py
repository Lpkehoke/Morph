from python.platform import NodeStorage, PluginManager, Logger
from python.ui import UiManager
import threading


def start():
    node_factory_registry = PluginManager()
    logger = Logger()

    node_storage = NodeStorage(node_factory_registry, logger)
    ui_manager = UiManager(node_storage, logger)

    def update():
        ui_manager.on_update()
        threading.Timer(1.0, update).start()

    update()
    ui_manager.start()


if __name__ == "__main__":
    start()

from python.platform import NodeStorage, NodeFactoryRegistry, Logger
from python.ui import UiManager

def start():
    node_factory_registry = NodeFactoryRegistry()
    logger = Logger()

    node_storage = NodeStorage(node_factory_registry, logger)
    ui_manager = UiManager(node_storage, logger)
    ui_manager.start()


if __name__ == "__main__":
    start()

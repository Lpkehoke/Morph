import threading

from python.platform import PlatformCore
from python.ui import UiManager


def start():
    platform_core = PlatformCore()
    ui_manager = UiManager(platform_core.node_storage(), platform_core.logger())

    def update():
        ui_manager.on_update()
        threading.Timer(1.0, update).start()

    update()
    ui_manager.start()


if __name__ == "__main__":
    start()

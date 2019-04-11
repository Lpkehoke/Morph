from PySide2.QtCore import Qt, Slot, Signal
from PySide2.QtWidgets import QWidget, QTextEdit, QHBoxLayout

from python.platform import Logger


class OutputViewer(QWidget):
    on_update = Signal()

    def __init__(self, logger: Logger, parent=None):
        super().__init__(parent)
        self._logger = logger
        self.setLayout(QHBoxLayout())

        self.output_textbox = QTextEdit()
        self.layout().addWidget(self.output_textbox)
        self._logger.subscribe(lambda: self.on_update.emit())
        self.on_update.connect(self._on_state, Qt.QueuedConnection)

    @Slot()
    def _on_state(self):
        state = self._logger.state()
        self.output_textbox.clear()

        for lr in state:
            msg = f'[{lr.timestamp}] {lr.severity}: {lr.message}'
            self.output_textbox.append(msg)

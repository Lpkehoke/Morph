from PySide2.QtCore import QObject, Signal, Slot, Qt
from PySide2.QtWidgets import QApplication

from python.core import NodeStorage, Logger, Severity
from .createnodedialog import CreateNodeDialog
from .stateviewer import StateViewer
from .outputviewer import OutputViewer
from .window import Window


class UiManager(QObject):
    on_state_updated = Signal()

    def __init__(self, node_storage: NodeStorage, logger: Logger):
        super().__init__()
        self.app = QApplication([])
        self.node_storage = node_storage
        self.logger = logger
        self.state = self.node_storage.state()
        self.widgets = []

        self.window = Window()

        self.create_node_dialog = CreateNodeDialog(lambda model, metadata: self.create_node(model, metadata),
                                                   self.window)
        self.create_node_dialog.hide()

    def create_node(self, model, metadata):
        self.node_storage.dispatch({
            'type': 'CreateNode',
            'model': model,
            'metadata': metadata
        })

        self.logger.log(Severity.Info, 'Called dispatch.')

        self.create_node_dialog.hide()

    def start(self):
        state_viewer = StateViewer(self.state)
        self.window.setCentralWidget(state_viewer)
        self.widgets.append(state_viewer)

        output_viewer = OutputViewer(self.logger)
        output_viewer.show()

        file_menu = self.window.menuBar().addMenu('File')
        node_menu = self.window.menuBar().addMenu('Node')

        create_node_action = node_menu.addAction('CreateNode')
        create_node_action.triggered.connect(lambda: self.create_node_dialog.show())

        self.on_state_updated.connect(self._on_update, Qt.QueuedConnection)

        self.window.show()

        self.node_storage.subscribe(lambda: self.on_update())
        self.app.exec_()

    def on_update(self):
        self.state = self.node_storage.state()
        self.on_state_updated.emit()


    @Slot()
    def _on_update(self):
        for w in self.widgets:
            w.on_state(self.state)

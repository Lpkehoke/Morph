from PySide2.QtWidgets import QDialog, QPushButton, QLineEdit, QLabel, QGridLayout


class RemoveNodeDialog(QDialog):
    def __init__(self, on_remove, parent):
        super().__init__(parent)

        self.on_remove = on_remove

        self.setLayout(QGridLayout())

        layout = self.layout()

        layout.addWidget(QLabel('NodeId'), 0, 0)

        self.node_id_textbox = QLineEdit('0')
        layout.addWidget(self.node_id_textbox, 0, 1)

        self.remove_button = QPushButton('Remove')
        layout.addWidget(self.remove_button, 1, 1)

        self.remove_button.clicked.connect(lambda: self._on_remove())

    def _on_remove(self):
        node_id = int(self.node_id_textbox.text())
        self.on_remove(node_id)

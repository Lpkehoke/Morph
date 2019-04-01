import itertools
import random

from PySide2.QtWidgets import QDialog, QPushButton, QLineEdit, QLabel, QGridLayout, QTextEdit, QCheckBox

first_names = ['Super', 'Awesome', 'Mega', 'Nasty', 'White', 'Sad', 'Disco', 'Sexy', 'Smelly', 'Hot',
               'Topless', 'Mad']
second_names = ['Pigeon', 'Worm', 'Nigga', 'Wombat', 'Dn4k', 'Cow', 'Monkey', 'Finger', 'Poop', 'Dog', 'Lama', 'Snake',
                'Vilain']

names = list(map(lambda elem: elem[0] + elem[1], itertools.product(first_names, second_names)))
for i in range(6666):
    random.shuffle(names)

names_iterator = itertools.cycle(names)


def random_name():
    return next(names_iterator)


class CreateNodeDialog(QDialog):
    def __init__(self, on_create, parent):
        super().__init__(parent)

        self.on_create = on_create

        self.setLayout(QGridLayout())

        layout = self.layout()

        layout.addWidget(QLabel('Model'), 0, 0)

        self.model_textbox = QLineEdit('Dummy')
        layout.addWidget(self.model_textbox, 0, 1)

        layout.addWidget(QLabel('Name'), 1, 0)

        self.name_textbox = QLineEdit(random_name())
        layout.addWidget(self.name_textbox, 1, 1)

        layout.addWidget(QLabel('ComputeEngine observable'), 2, 0)

        self.observable_checkbox = QCheckBox()
        layout.addWidget(self.observable_checkbox, 2, 1)

        layout.addWidget(QLabel('Custom Metadata'), 3, 0)

        self.metadata_textbox = QTextEdit('{}')
        layout.addWidget(self.metadata_textbox, 3, 1)

        self.create_button = QPushButton('Create')
        layout.addWidget(self.create_button, 4, 1)

        self.create_button.clicked.connect(lambda: self._on_create())

    def _on_create(self):
        model = self.model_textbox.text()
        metadata = eval(self.metadata_textbox.toPlainText())
        metadata['name'] = self.name_textbox.text()

        if self.observable_checkbox.isChecked():
            metadata['computeengine.observable'] = True

        self.on_create(model, metadata)

        self.name_textbox.setText(random_name())

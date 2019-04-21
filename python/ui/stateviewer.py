from PySide2.QtCore import QAbstractTableModel, QModelIndex, Qt
from PySide2.QtWidgets import QWidget, QTableView, QHeaderView, QGridLayout
from PySide2.QtGui import QColor

from python.morph.core import NodeStorageState


class BaseTableModel(QAbstractTableModel):
    def __init__(self, state, parent):
        super().__init__(parent)
        self.rows = []
        self.on_state(state)
        self.header = ['key', 'value']

    def on_state(self, state):
        raise Exception('Not implemented.')

    def rowCount(self, parent=QModelIndex()):
        return len(self.rows)

    def columnCount(self, parent=QModelIndex()):
        return 2

    def headerData(self, section, orientation, role):
        if role != Qt.DisplayRole:
            return None
        if orientation == Qt.Horizontal:
            return self.header[section]
        else:
            return "{}".format(section)

    def data(self, index, role=Qt.DisplayRole):
        column = index.column()
        row = index.row()

        if role == Qt.DisplayRole:
            return str(self.rows[row][column])
        elif role == Qt.BackgroundRole:
            return QColor(Qt.white)
        elif role == Qt.TextAlignmentRole:
            return Qt.AlignRight

        return None


class NodeTableModel(BaseTableModel):
    def __init__(self, state, parent):
        super().__init__(state, parent)
        self.header = ['NodeId', 'Node']

    def on_state(self, state):
        self.rows = []
        for node_id, node in state.nodes:
            self.rows.append((node_id, node))


class MetadataTableModel(BaseTableModel):
    def __init__(self, state, parent):
        super().__init__(state, parent)
        self.header = ['NodeId', 'Metadata']

    def on_state(self, state):
        self.rows = []
        for node_id, metadata in state.node_metadata:
            self.rows.append((node_id, metadata))


class KnobTableModel(BaseTableModel):
    def __init__(self, state, parent):
        super().__init__(state, parent)
        self.header = ['KnobId', 'Knob']

    def on_state(self, state):
        self.rows = []
        for knob_id, knob in state.knobs:
            self.rows.append((knob_id, knob))


class AttributeTableModel(BaseTableModel):
    def __init__(self, state, parent):
        super().__init__(state, parent)
        self.header = ['AttrId', 'Attribute']

    def on_state(self, state):
        self.rows = []
        for attr_id, attribute in state.attributes:
            self.rows.append((attr_id, attribute))


class StateViewer(QWidget):
    def __init__(self, state, parent=None):
        super().__init__(parent)

        #   init nodes table
        self.nodes_table = QTableView(self)
        self.nodes_table.setModel(NodeTableModel(state, self))
        self.nodes_table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)

        self.metadata_table = QTableView(self)
        self.metadata_table.setModel(MetadataTableModel(state, self))
        self.metadata_table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)

        self.knob_table = QTableView(self)
        self.knob_table.setModel(KnobTableModel(state, self))
        self.knob_table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)

        self.attr_table = QTableView(self)
        self.attr_table.setModel(AttributeTableModel(state, self))
        self.attr_table.horizontalHeader().setSectionResizeMode(QHeaderView.Stretch)

        self.setLayout(QGridLayout())
        self.layout().addWidget(self.nodes_table, 0, 0)
        self.layout().addWidget(self.metadata_table, 0, 1)
        self.layout().addWidget(self.knob_table, 1, 0)
        self.layout().addWidget(self.attr_table, 1, 1)

    def on_state(self, state: NodeStorageState):
        self.nodes_table.setModel(NodeTableModel(state, None))
        self.metadata_table.setModel(MetadataTableModel(state, None))
        self.knob_table.setModel(KnobTableModel(state, None))
        self.attr_table.setModel(AttributeTableModel(state, None))

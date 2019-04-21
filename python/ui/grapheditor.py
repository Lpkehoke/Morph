from PySide2.QtCore import QPoint
from PySide2.QtGui import QPainter
from PySide2.QtWidgets import QOpenGLWidget

from python.morph.gl import init_opengl
from python.morph.ui import Viewport


class GraphEditor(QOpenGLWidget):
    def __init__(self, get_state, parent=None):
        super().__init__(parent)
        self._get_state = get_state

    def initializeGL(self):
        init_opengl()

    def paintGL(self):
        state = self._get_state()
        viewport = Viewport()
        viewport.render(state)

        painter = QPainter(self)

        # painter.fillRect(self.rect(), QColor.fromRgb(33, 33, 33))

        for node_id, node in state.nodes:
            painter.drawText(QPoint(50, 50), state.node_metadata[node_id]['name'])

import time
import unittest

from python.platform import NodeStorage, NodeFactoryRegistry, KnobModelRegistry, Logger


class TestNodeStorage(unittest.TestCase):

    def setUp(self):
        self.node_factory_registry = NodeFactoryRegistry()
        self.logger = Logger()
        self.knob_model_registry = KnobModelRegistry()
        self.node_storage = NodeStorage(
            self.node_factory_registry,
            self.knob_model_registry,
            self.logger)

    def test_dispatch(self):
        node_id = 100
        node_metadata = {
            'key': 'cool',
            'float': 1.1254,
            'int_as_float': 26
        }

        self.node_storage.dispatch({
            "type": "CreateNode",
            "model": "Dummy",
            "id": node_id,
            "metadata": node_metadata
        })

        time.sleep(1)

        #   TODO: test callback is getting called

        state = self.node_storage.state()

        self.assertIsNotNone(state.nodes[node_id])

        meta = state.node_metadata[node_id]
        for key, value in meta:
            self.assertEqual(node_metadata[key], value)


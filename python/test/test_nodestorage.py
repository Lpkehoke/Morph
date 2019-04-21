import time
import unittest

from python.morph.core import NodeStorage, NodeFactoryRegistry, Logger


class TestNodeStorage(unittest.TestCase):

    def setUp(self):
        self.node_factory_registry = NodeFactoryRegistry()
        self.logger = Logger()
        self.node_storage = NodeStorage(
            self.node_factory_registry,
            self.logger)

    def test_dispatch(self):
        node_metadata = {
            'key': 'cool',
            'float': 1.1254,
            'int_as_float': 26
        }

        self.node_storage.dispatch({
            "type": "CreateNode",
            "model": "Dummy",
            "metadata": node_metadata
        })

        time.sleep(1)

        #   TODO: test callback is getting called

        state = self.node_storage.state()

        self.assertIsNotNone(state.nodes[0])

        meta = state.node_metadata[0]
        for key, value in meta:
            self.assertEqual(node_metadata[key], value)


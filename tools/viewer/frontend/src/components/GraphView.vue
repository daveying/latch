<script setup lang="ts">
import { onMounted, ref } from "vue"
import mxgraph from './mxgraph'

const mxGraph = mxgraph.mxGraph
const mxClient = mxgraph.mxClient
const mxShape = mxgraph.mxShape
const mxUtils = mxgraph.mxUtils
const mxConnectionConstraint = mxgraph.mxConnectionConstraint
const mxPoint = mxgraph.mxPoint
const mxPolyline = mxgraph.mxPolyline
const mxEvent = mxgraph.mxEvent
const mxCellState = mxgraph.mxCellState
const mxRubberband = mxgraph.mxRubberband
const mxHierarchicalLayout = mxgraph.mxHierarchicalLayout
const mxConstants = mxgraph.mxConstants

if (!mxClient.isBrowserSupported())
{
  // Displays an error message if the browser is not supported.
  mxUtils.error('Browser is not supported!', 200, false);
}

mxGraph.prototype.getAllConnectionConstraints = function(terminal, source?: boolean | undefined) {
  if (terminal.shape.constraints != null)
      return terminal.shape.constraints
  if (terminal != null && terminal.shape != null) {
    if (terminal.shape.stencil != null) {
      if (terminal.shape.stencil.constraints != null) {
        return terminal.shape.stencil.constraints
      }
    }
  }
  else if (terminal.shape.constraints != null) {
    return terminal.shape.constraints
  }
  return null
}
mxShape.prototype.constraints = [new mxConnectionConstraint(new mxPoint(0.25, 0), true),
                                 new mxConnectionConstraint(new mxPoint(0.5, 0), true),
                                 new mxConnectionConstraint(new mxPoint(0.75, 0), true),
                                 new mxConnectionConstraint(new mxPoint(0, 0.25), true),
                                 new mxConnectionConstraint(new mxPoint(0, 0.5), true),
                                 new mxConnectionConstraint(new mxPoint(0, 0.75), true),
                                 new mxConnectionConstraint(new mxPoint(1, 0.25), true),
                                 new mxConnectionConstraint(new mxPoint(1, 0.5), true),
                                 new mxConnectionConstraint(new mxPoint(1, 0.75), true),
                                 new mxConnectionConstraint(new mxPoint(0.25, 1), true),
                                 new mxConnectionConstraint(new mxPoint(0.5, 1), true),
                                 new mxConnectionConstraint(new mxPoint(0.75, 1), true)];
mxPolyline.prototype.constraints = null
// Program starts here. Creates a sample graph in the
// DOM node with the specified ID. This function is invoked
// from the onLoad event handler of the document (see below).
function main(container: any)
{
  // Checks if the browser is supported
  if (!mxClient.isBrowserSupported())
  {
    // Displays an error message if the browser is not supported.
    mxUtils.error('Browser is not supported!', 200, false);
  }
  else
  {
    // Disables the built-in context menu
    mxEvent.disableContextMenu(container);
    // Creates the graph inside the given container
    var graph = new mxGraph(container);
    graph.setConnectable(true);
    // Enables connect preview for the default edge style
    graph.connectionHandler.createEdgeState = function(me)
    {
      var edge = graph.createEdge(null, null, null, null, null);
      return new mxCellState(this.graph.view, edge, this.graph.getCellStyle(edge));
    };
    // Enables rubberband selection
    new mxRubberband(graph);
    // Gets the default parent for inserting new cells. This
    // is normally the first child of the root (ie. layer 0).
    var parent = graph.getDefaultParent();
    // Adds cells to the model in a single step
    graph.getModel().beginUpdate();

    var layout = new mxHierarchicalLayout(graph, mxConstants.DIRECTION_WEST)
    try
    {
      var v1 = graph.insertVertex(parent, null, 'Hello,', 0, 0, 80, 30, "rounded;strokeColor=red;fillColor=green");
      var v2 = graph.insertVertex(parent, null, 'World!', 0, 0, 80, 30);
      var v3 = graph.insertVertex(parent, null, 'World2!', 100, 0, 180, 100);
      var v4 = graph.insertVertex(v3, null, "a", 0, 10, 40, 15);
      var v5 = graph.insertVertex(v3, null, "b", 0, 12, 40, 15);
      var v6 = graph.insertVertex(v3, null, "c", 0, 14, 40, 15);
      graph.insertEdge(parent, null, '', v1, v2);
      graph.insertEdge(parent, null, '', v1, v3);
      graph.insertEdge(v3, null, '', v4, v5);
      graph.insertEdge(v3, null, '', v4, v6);
      graph.insertEdge(v3, null, '', v3, v4);
      layout.execute(parent, [v1, v2, v3])
      layout.execute(v3, [v4, v5, v6])
    }
    finally
    {
      // Updates the display
      graph.getModel().endUpdate();
    }
  }
}

const graphContainer = ref(null)

onMounted(() => {
  main(graphContainer.value);
})

</script>

<template>
  <div ref="graphContainer" class="graphContainer"></div>
</template>

<style>
.graphContainer {
  height:100%;
  position:relative;
  overflow:hidden;
  background:url('src/assets/grid.gif');
  cursor:default;
}
</style>

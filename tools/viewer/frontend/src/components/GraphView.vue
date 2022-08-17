<script lang="ts">
import { defineComponent } from 'vue'
import mxgraph from './mxgraph'

if (!mxgraph.mxClient.isBrowserSupported())
{
    // Displays an error message if the browser is not supported.
    mxgraph.mxUtils.error('Browser is not supported!', 200, false);
}

// Program starts here. Creates a sample graph in the
// DOM node with the specified ID. This function is invoked
// from the onLoad event handler of the document (see below).
function main(container: any)
{
	// Checks if the browser is supported
	if (!mxgraph.mxClient.isBrowserSupported())
	{
		// Displays an error message if the browser is not supported.
		mxgraph.mxUtils.error('Browser is not supported!', 200, false);
	}
	else
	{
		// Disables the built-in context menu
		mxgraph.mxEvent.disableContextMenu(container);

		// Creates the graph inside the given container
		var graph = new mxgraph.mxGraph(container);

		// Enables rubberband selection
		new mxgraph.mxRubberband(graph);

		// Gets the default parent for inserting new cells. This
		// is normally the first child of the root (ie. layer 0).
		var parent = graph.getDefaultParent();

		// Adds cells to the model in a single step
		graph.getModel().beginUpdate();
		try
		{
			var v1 = graph.insertVertex(parent, null, 'Hello,', 20, 20, 80, 30, "rounded;strokeColor=red;fillColor=green");
			var v2 = graph.insertVertex(parent, null, 'World!', 200, 150, 80, 30);
			graph.insertEdge(parent, null, '', v1, v2);
		}
		finally
		{
			// Updates the display
			graph.getModel().endUpdate();
		}
	}
}

export default defineComponent({
    mounted() {
		main(document.getElementById("graphContainer"));
    }
})

</script>

<template>
  <div id="graphContainer"></div>
</template>

<style>
#graphContainer {
    height:100%;
    position:relative;
    overflow:hidden;
    background:url('src/assets/grid.gif');
    cursor:default;
}
</style>

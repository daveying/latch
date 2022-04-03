<script lang="ts">
import { defineComponent } from 'vue'
import mxgraph from './mxgraph'

if (!mxgraph.mxClient.isBrowserSupported())
{
    // Displays an error message if the browser is not supported.
    mxgraph.mxUtils.error('Browser is not supported!', 200, false);
}

export default defineComponent({
    mounted() {
    }
})

</script>

<template>
  <div id="view">abc</div>
</template>

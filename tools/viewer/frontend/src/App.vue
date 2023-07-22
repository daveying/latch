<script setup lang="ts">
import GraphView from './components/GraphView.vue'
import GraphViewV2 from './components/GraphViewV2.vue'
import descriptors from './assets/descriptors.json'
import { ref, computed } from 'vue'
import { Loader } from './descriptor/loader'

const selectedComponent = ref('');

const loader = new Loader(descriptors.descriptors);

</script>

<template>
  <main>
    <el-select v-model="selectedComponent" class="m-2" placeholder="Select component">
      <el-option
        v-for="desc in loader.descriptors.values()"
        :key="desc.type"
        :label="desc.type"
        :value="desc.type"
      />
    </el-select>
    <div id="container">
      <div id="first">
        <GraphView />
      </div>
      <div id="second">
        <GraphViewV2 />
      </div>
    </div>
  </main>
</template>

<style>
@import './assets/base.css';
#container {
    height: 100%;
}
#first {
    height: 50%;
    border: 2px solid grey;
}
#second {
    height: 50%;
    border: 2px solid grey;
}
</style>

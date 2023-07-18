import mx from 'mxgraph'

// WAR because of the strict mode
(window as any)['mxBasePath'] = 'src/assets/';
(window as any)['mxLoadResources'] = true;
(window as any)['mxResourceExtension'] = '.txt';
(window as any)['mxForceIncludes'] = false;
(window as any)['mxLoadStylesheets'] = true;

// mx is the function returned from <viewer>/node_modules/mxgraph/javascript/dist/build.json:10
// the empty {} passed into mx() is the opts
// mxgraph will be the object __mxOutput
const mxgraph = mx({});

export default mxgraph;

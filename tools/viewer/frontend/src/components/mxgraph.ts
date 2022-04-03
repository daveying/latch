import mx from 'mxgraph'

// WAR because of the strict mode
(window as any)['mxBasePath'] = 'node_modules/mxgraph/javascript/src';
(window as any)['mxLoadResources'] = true;
(window as any)['mxResourceExtension'] = '.txt';
(window as any)['mxForceIncludes'] = false;
(window as any)['mxLoadStylesheets'] = true;

const mxgraph = mx({});

export default mxgraph;

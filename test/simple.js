const tap = require('tap');
const fbx = require('../index.js');

function print_node ( indent, node ) {
  console.log( ' '.repeat(indent) + '|-' + node.name + ` [${node.type}]` );
  node.children.forEach(child => {
    print_node(indent+1, child);
  });
}

tap.test('load', {timeout: 0}, t => {
  let scene = fbx.load('./models/xsi_man.fbx');

  setTimeout(() => {
    console.log('\n');

    scene.nodes.forEach(node => {
      print_node(0, node);
    });

    console.log(scene);

    scene.meshes.forEach(mesh => {
      mesh.indices = fbx.calculateIndices(mesh);
    });

    require('fs').writeFileSync('./tmp/output.json', JSON.stringify(scene, null, 2));
  }, 10);

  t.end();
});

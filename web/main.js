import * as THREE from 'three';
import { OrbitControls } from "three/examples/jsm/controls/OrbitControls.js";//orbit control to change camera view
import GUI from "lil-gui"; //interactive control panel
import Stats from "stats.js"; //importing for fps stats to imitate an engine

    const scene = new THREE.Scene();
    scene.background = new THREE.Color(0x111827);

    const camera = new THREE.PerspectiveCamera(45, window.innerWidth / window.innerHeight, 0.1, 1000);
    camera.position.set(0, 0, 55);
    camera.lookAt(0, 0, 0);
     
    const axesHelper=new THREE.AxesHelper(15);
    scene.add(axesHelper); //added axeshelper

    const gridHelper=new THREE.GridHelper(100,100);
    scene.add(gridHelper);
    gridHelper.position.y = -7.95;

    const renderer = new THREE.WebGLRenderer({ antialias: true });
    renderer.setSize(window.innerWidth, window.innerHeight);
    renderer.shadowMap.enabled = true;
    renderer.shadowMap.type = THREE.PCFSoftShadowMap; //whilst polish

    document.body.appendChild(renderer.domElement);

    const stats = new Stats();
    stats.showPanel(0); //fps tracking
    document.body.appendChild(stats.dom);

    stats.dom.style.transform="scale(1.5)";
    stats.dom.style.transformOrigin="top left";

    const controls = new OrbitControls(camera, renderer.domElement); //start listening to mouse for input and updates the cam about (0,8,0)
    controls.enableDamping = true;
    controls.dampingFactor = 0.05; //smoothness 

    controls.minDistance = 10;
    controls.maxDistance = 100; //zooming into the landscape

    controls.target.set(0, 0, 0);
    controls.update(); 

    const ambientLight = new THREE.AmbientLight(0xffffff, 0.4);
    scene.add(ambientLight);

    const dirLight = new THREE.DirectionalLight(0xffffff, 2);
    dirLight.position.set(15, 25, 15);
    //adding shadows
    dirLight.castShadow=true;
    //shadow quality
    dirLight.shadow.mapSize.width=2048;
    dirLight.shadow.mapSize.height = 2048;

    dirLight.shadow.camera.left = -30;
    dirLight.shadow.camera.right = 30;
    dirLight.shadow.camera.top = 30;
    dirLight.shadow.camera.bottom = -30;

    dirLight.shadow.camera.near = 1;
    dirLight.shadow.camera.far = 80;
    scene.add(dirLight);

    //adding ground properties - has PlaneGeometry and Material

    const groundGeometry=new THREE.PlaneGeometry(100,100);
    const groundMaterial=new THREE.MeshStandardMaterial({
        color: 0x666666,
        roughness: 0.9,
        metalness: 0.0
    });
    //creating the mesh
    const ground = new THREE.Mesh(groundGeometry, groundMaterial);
    ground.rotation.x = -Math.PI / 2; //default verti
    ground.position.y = -8;
    ground.receiveShadow=true;
    scene.add(ground);

    const gui = new GUI();
    const settings = {
    gravity: -9.81,
    timeScale: 1, //for slower or faster demos
    paused: false,

    //allowing user to spawn their own balls
    spawnX: 0,
    spawnY: 40,
    spawnZ: 0,

    radius:1,
    mass: 1,
    restitution: 0.5,//setting default values for the balls
    };

    createPhysicsModule().then((Module) => {
      let world = new Module.PhysicsWorld();

      world.setGravity(settings.gravity); //default is -9.8
      gui.add(settings, "gravity", -20,0,0.1) //min max and step
      .name("World Gravity")
      .onChange((value) => {
        world.setGravity(value); //callback
      });

      gui.add(settings, "timeScale", 0,5,0.1)
     .name("Time Scale"); //this is the time scale slider

     gui.add(settings, "paused").name("Pause");//enabling pause and resume feature in the world

     //adding the spawn object characteristic sliders
     gui.add(settings, "spawnX", -25,25,0.5).name("Spawn X co-ordinate");
     gui.add(settings, "spawnY", 0,60,0.5).name("Spawn Y co-ordinate");
     gui.add(settings, "spawnZ", -25,25,0.5).name("Spawn Z co-ordinate");

     gui.add(settings, "radius", 0.5, 3, 0.1).name("Radius");
     gui.add(settings, "mass", 0.1, 10, 0.1).name("Mass");
     gui.add(settings, "restitution", 0, 1, 0.05).name("Restitution");

      const meshes = [];

      function spawnBall(x, y, z, radius = 1, mass = 1, restitution = 0.6) {

      const body = new Module.RigidBody(
      new Module.Vector3(x, y, z),
      new Module.Vector3(0, 0, 0),
      radius,
      mass,
      restitution
      );

      world.addBody(body);

     const geometry = new THREE.SphereGeometry(radius, 32, 32);
     const material = new THREE.MeshStandardMaterial({
     color: 0x4da6ff,
     roughness: 0.45,
     metalness: 0.15
     });

     const sphereMesh = new THREE.Mesh(geometry, material);

     sphereMesh.castShadow = true;
     sphereMesh.receiveShadow = true;//setting shadow properties

     scene.add(sphereMesh);
     meshes.push(sphereMesh);
    }
        settings.spawn = () => {
             const offset = () => (Math.random() - 0.5) * 0.5;//added a small offset
        spawnBall(
        settings.spawnX + offset(),
        settings.spawnY,
        settings.spawnZ + offset(),
        settings.radius,
        settings.mass,
        settings.restitution
        );
        }
        gui.add(settings, "spawn").name("Spawn Ball");

        //calling balls or spawning them when needed default
        spawnBall(0.0, 10.0, 0.0, 1.0, 1.0, 0.70);
        spawnBall(-2.5, 13.0, 1.5, 0.9, 0.8, 0.80);
        spawnBall(2.8, 16.0, -1.2, 1.2, 1.5, 0.55);

        spawnBall(-4.5, 20.0, 3.0, 1.1, 1.3, 0.65);
        spawnBall(4.2, 22.0, -2.8, 0.8, 0.6, 0.90);
        spawnBall(0.0, 25.0, 2.5, 1.4, 2.0, 0.45);

        spawnBall(-6.5, 29.0, -1.5, 1.0, 1.0, 0.75);
        spawnBall(6.0, 32.0, 2.2, 0.7, 0.5, 0.95);
        spawnBall(-1.8, 35.0, -3.5, 1.3, 1.8, 0.60);

        spawnBall(2.5, 39.0, 3.5, 0.9, 0.9, 0.80);
        spawnBall(-8.0, 42.0, 0.0, 1.5, 2.5, 0.40);
        spawnBall(8.0, 45.0, -0.5, 0.8, 0.7, 0.90);

        spawnBall(0.0, 49.0, 4.5, 1.1, 1.2, 0.70);
        spawnBall(-3.5, 53.0, -4.0, 1.0, 1.0, 0.65);
        spawnBall(3.8, 57.0, 1.0, 1.2, 1.6, 0.55);

      function animate() {
        stats.begin();  //keep track of stats
        requestAnimationFrame(animate);
        if(!settings.paused) //checker for pause
        {
        world.step(0.016 * settings.timeScale); //frames modification
        }
        const positions = world.getPositions();

        for (let i = 0; i < meshes.length; i++) {
          const x = positions.get(i * 3);
          const y = positions.get(i * 3 + 1);
          const z = positions.get(i * 3 + 2);
          meshes[i].position.set(x, y, z);
        }
        controls.update();
        renderer.render(scene, camera);
        stats.end();
      }
      animate();
    });
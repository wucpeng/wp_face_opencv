const path = require('path');
let printMem = ()=> {
    var mem = process.memoryUsage();
    var format = function(bytes) {
        return (bytes/1024/1024).toFixed(2)+'MB';
    };
    console.log('Process1: heapTotal '+format(mem.heapTotal) + ' heapUsed ' + format(mem.heapUsed) + ' rss ' + format(mem.rss));
}
printMem()
var addon = require('bindings')('wp_face_opencv');

let start = Date.now();
let nodes = new addon.NODES();

printMem()

const urls = [
     "https://min.haizitong.com/2/ali/i/fbad726e022742a78a2666580a9a0f0f139317", //大史
    //  "http://image.haizitong.com/cc123be3667143e3af5c53a5906c6b42", //一个小女孩
    // "http://upload-file.haizitong.com/api/getFile/119c44a5-193f-4953-b59c-c1e55cd29f1a",// 16位4通道  
    //  "http://upload-file.haizitong.com/api/getCacheFile/62e23c73991e856c4411bbe4",//星空图
    //  "http://min.haizitong.com/2/ali/i/516a55e452564ab3b500909980cb4994932439", // 一家4人
    //  "https://upload-file.haizitong.com/api/getCacheFile/6464809130b3072db64e505d", //横图
    //  "https://upload-file.haizitong.com/api/getCacheFile/64647e4b30b3072db64e5030", // 校运会
    //  "http://upload-file.haizitong.com/api/getCacheFile/6354b922991e85059ca07eb3", //游乐场

]
for(let o of urls) { //flower , "rabbit" head
    let xx2 = nodes.cnnFaceDetectUrl(o, "head", __dirname);
    console.log("cvFaceDetectUrl", xx2,  Date.now() - start);
    // let cv2 = nodes.cvFaceDetectUrl(o, "head", __dirname);
    // console.log("cvFaceDetectUrl", cv2,  Date.now() - start);
    //printMem();
}

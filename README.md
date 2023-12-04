#wp_face_opencv
人脸检测 贴人脸 并保存图片

## How to use the code
npm i wp_face_opencv

const wpFace = require("wp_face_opencv")

let nodes = new wpFace.NODES();

let result = nodes.cnnFaceDetectUrl(url, key, output); 
输入:
url: 图片地址
key: 贴图关键字  flower, head, rabbit
output: 图片保存文件夹

输出: result {faceNum: 人脸数量, faceRects: [{x, y, witdh, height}], 人脸坐标}



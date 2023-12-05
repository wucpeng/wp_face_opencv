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


##环境准备 cmake cmake-js opencv curl 
curl -O http://static-res.haizitong.com/nodejs/cmake-3.3.2.tar.gz
tar xzvf cmake-3.3.2.tar.gz
cd cmake-3.3.2 
./bootstrap
gmake
make install
cd ../
echo "init opencv"
apt-get install -y build-essential libgtk2.0-dev libavcodec-dev libavformat-dev libjpeg.dev libtiff4.dev libswscale-dev libjasper-dev
curl -O http://static-res.haizitong.com/nodejs/opencv-3.4.15.zip
unzip opencv-3.4.15.zip
cd opencv-3.4.15/
mkdir release
cd release/
cmake ../
make
make install

cd ../../

apt install -y libcurl4-openssl-dev

npm i -g cmake-js
ln -s /opt/node/bin/cmake-js /usr/local/bin/cmake-js
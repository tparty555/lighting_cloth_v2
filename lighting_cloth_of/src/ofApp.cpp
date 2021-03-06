#include "ofApp.h"
#define RECIEVE_PORT 8000

int tmp=-1;
//--------------------------------------------------------------
void ofApp::setup(){
    //音関連
    fft.setup(pow(2.0,12.0));
    //画像関連
    TParty.load("images/TParty.png");
    cloth.load("images/cloth.png");
    cloth_image_positon.set(500,50);//cloth.pngの位置
    cloth_image_size.set(350,350*cloth_image_aspect_ratio);//cloth.pngのサイズ
    //GUI
    ofBackground(0,0,0);
    gui.setup();
    gui.add(hue_first.setup("hue_first",0,0,360));
    gui.add(hue_second.setup("hue_second",240,0,360));
    gui.add(value.setup("value",240,221,240));
    gui.add(sound_volume_ratio.setup("sound ratio",1,0,1));
    //gui.add(sound_volume_min.setup("sound_volume_min",10,0,sound_volume_500));
    //gui.add(sound_volume_max.setup("sound_volume_max",300,0,sound_volume_500));
    gui.add(LowPass.setup("LowPass(0~2048)",50,0,2048));
    gui.add(HighPass.setup("HighPass(0~2048)",500,0,2048));
    gui.add(s_r_attenu.setup("s_r_attenu(when mic on)",1,0,1));
    gui.add(s_r_multi.setup("s_r_multi(when mic on)",1,1,30));
    gui.add(fft_hue_min.setup("fft_hue_min",58,0,100));
    gui.add(fft_hue_max.setup("fft_hue_max",85,0,100));
    gui.add(using_volume_button.setup("using_volume_button"));
    gui.add(using_fft_hue_button.setup("using_fft_hue_button"));
    fft_draw_size.set(400,200);
    // 描画系設定
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    ofTrueTypeFont::setGlobalDpi(72);
    mic_on_or_off.load("Impact.ttf", 18);
    fft_hue_on_or_off.load("Impact.ttf", 18);
    // シリアル通信
    //単数の時
    /*
    serialArduino.listDevices();
    if(!serialArduino.setup(1, 9600)) {
        // log and error and leave
        ofLogError() << "could not open serial port - listing serial devices";
        serialArduino.listDevices();
        OF_EXIT_APP(0);
    }
     */
    //複数の時
    serialArduino[0].listDevices();
    if(!serialArduino[0].setup(5, 9600)) {
        // log and error and leave
        ofLogError() << "0: could not open serial port - listing serial devices";
        //serialArduino1.listDevices();
        OF_EXIT_APP(0);
    }
    if(!serialArduino[1].setup(1, 9600)) {
        // log and error and leave
        ofLogError() << "1: could not open serial port - listing serial devices";
        //serialArduino2.listDevices();
        OF_EXIT_APP(0);
    }
    if(!serialArduino[2].setup(3, 9600)) {
        // log and error and leave
        ofLogError() << "2: could not open serial port - listing serial devices";
        //serialArduino3.listDevices();
        OF_EXIT_APP(0);
    }
    
    
    
    //色設定
    
    //OSC
    receiver.setup(RECIEVE_PORT);
}

//--------------------------------------------------------------
void ofApp::update(){
    //OSC/////////////////////////
    ////////////ここからOSCendまでコメントアウトするとGUIでも使えるようになる
    
    OSCrecv();
    hue_first=osc_in[1]*360;
    hue_second=osc_in[2]*360;
    value=ofMap(osc_in[0],0,1,221,240);
    sound_volume_ratio = osc_in[5];
    if(mode_switch[0]=="bang"){
        mode=205;
    }
    if(mode_switch[1]=="bang"){
        mode=206;
    }
    if(mode_switch[2]=="bang"){
        mode=203;
    }
    if(mode_switch[3]=="bang"){
        mode=204;
    }
    if(mode_switch[3]=="bang"){
        mode=204;
    }
    if(mode_switch[4]=="bang"){
        mode=208;
    }
    if(mode_switch[5]=="bang"){
        mode=209;
    }
    if(mode_switch[6]=="bang"){
        mode=207;
    }
    if(mode_switch[7]=="bang"){
        if(using_volume==true){
            using_volume=false;
        }else{
            using_volume=true;
        }
    }
    if(mode_switch[8]=="bang"){
        if(using_fft_hue==true){
            using_fft_hue=false;
        }else{
            using_fft_hue=true;
        }
    }
    
    /////////////////OSCend ここまでコメントアウトするとGUIから使えるようになる
    //////////////////////////////
    
    //音関連////////////////////////
    fft.update();
    ///////////////////////////////
    
    //色の値/////////////////////////デフォルトではhue_firstを送る
    hue_first_send=ofMap(hue_first,0,360,1,100);
    hue_second_send=ofMap(hue_second,0,360,101,200);
    ////////////////////////////////
    
    
    
    //モードについて////////////////////
    //mode=205;
    /////////////////////////////////
    
    
    
    //モードを戻す/これはupdateの最後////
    
    ////////////////////////////////
    
    //ボタンの処理
    if(using_volume_button){
        if(using_volume==true){
            using_volume=false;
        }else{
            using_volume=true;
        }
    }
    
    if(using_fft_hue_button){
        if(using_fft_hue==true){
            using_fft_hue=false;
        }else{
            using_fft_hue=true;
        }
    }
    //OSC戻す
    for(int i=0;i<9;i++){
        mode_switch[i]=" ";
    }
    mode_volume=" ";
}

//--------------------------------------------------------------
void ofApp::draw(){
    //音関連///////////////////////////
    vector<float> buffer;
    buffer=fft.getBins();
    ofNoFill();
    ofSetLineWidth(2.0);
    ofBeginShape();
    for(int i=0;i<buffer.size();i++){
        if(i>=LowPass&&i<=HighPass){
            if(buffer[i]>frequency_volume_max_volume){
                frequency_volume_max_volume=buffer[i];
                frequency_volume_max=i;
            }
            buffer[i]*=sound_volume_ratio;
            buffer_sum+=buffer[i];
            float x=ofMap(i,0,buffer.size(),0,ofGetWidth());
            float y=ofMap(buffer[i],0,1,ofGetHeight(),0);
            ofVertex(x, y/3+ofGetHeight()-300);
        }
    }
    ofEndShape();
    sound_volume_0to1=ofMap(buffer_sum,0,sound_volume_max,0,1);
    if(sound_volume_0to1>=1){
        sound_volume_0to1=1;
    }
    //ofDrawBitmapString(sound_volume_0to1,200,200);//音量の表示
    string msg_mode="mode: "+ofToString(mode);
    ofDrawBitmapString(msg_mode,200,500);
    string msg_using_volume = ofToString(int(using_volume)) + " do you use mic? 0:NO 1:YES";
    ofDrawBitmapString(msg_using_volume,200,550);
    ofDrawBitmapString(buffer_sum,200,700);
   
    for(int i=8;i>=0;i--){
        frequency_volume_max_ten_average_array[i+1]=frequency_volume_max_ten_average_array[i];
    }
    frequency_volume_max_ten_average_array[0]=frequency_volume_max;
    for(int i=0;i<10;i++){
        frequency_volume_max_ten_average+=frequency_volume_max_ten_average_array[i];
    }
    frequency_volume_max_ten_average/=10;
    
    string msg_frequency="frequency_volume_max: "+ofToString(int(frequency_volume_max))+"   frequency_volume_max_ten_average: "+ofToString(int(frequency_volume_max_ten_average));
    ofDrawBitmapString(msg_frequency,200,800);
    ofSetColor(255, 0, 0);
    ofFill();
    ofRect(ofMap(frequency_volume_max_ten_average,0,buffer.size(),0,ofGetWidth()),ofMap(frequency_volume_max_volume,0,1,ofGetHeight(),0)/3+ofGetHeight()-300, 5, 1000);
    ofNoFill();
    ofSetColor(255, 255, 255);
    fft_hue=ofMap(frequency_volume_max_ten_average, LowPass, HighPass, 1, 100);
    string msg_fft_hue="send_fft_hue(1~100): "+ofToString(send_fft_hue);
    ofDrawBitmapString(msg_fft_hue,200,850);
    string msg_fft_hue_bool=ofToString(using_fft_hue)+" do you use fft_hue?";
    ofDrawBitmapString(msg_fft_hue_bool,400,850);
    
    int volume_arduino_send;
    volume_arduino_send=ofMap(buffer_sum*s_r_multi*s_r_attenu, 0, 100, 221, 240);
    if(volume_arduino_send<221){
        volume_arduino_send=221;
    }else if(volume_arduino_send>240){
        volume_arduino_send=240;
    }
    string msg_volume_arduino_send="volume_arduino_send: "+ofToString(int(volume_arduino_send));
    ofDrawBitmapString(msg_volume_arduino_send,400,600);
    ///////////////////////////////////
    
    //シリアル通信////////////////////
    //単数の時
    /*
    serialArduino.writeByte(Byte(hue_first_send));//hue//serialArduino.writeByte(Byte(hue_send));
    serialArduino.writeByte(Byte(hue_second_send));
    serialArduino.writeByte(Byte(mode));//serialArduino.writeByte(Byte(mode));
    serialArduino.writeByte(Byte(value));
    */
    //複数の時
    send_fft_hue=ofMap(fft_hue, 0, 100, fft_hue_min, fft_hue_max);
    for(int i=0;i<3;i++){
        if(using_fft_hue==true){
            serialArduino[i].writeByte(Byte(send_fft_hue));
            std::cout<<send_fft_hue;
            serialArduino[i].writeByte(Byte(send_fft_hue+100));
            std::cout<<" "<<send_fft_hue+100;
        }else{
            serialArduino[i].writeByte(Byte(hue_first_send));
            std::cout<<hue_first_send;
            serialArduino[i].writeByte(Byte(hue_second_send));
            std::cout<<" "<<hue_second_send;
        }
        serialArduino[i].writeByte(Byte(mode));//serialArduino.writeByte(Byte(mode));
        std::cout<<" "<<mode;
        if(using_volume==true){
            serialArduino[i].writeByte(Byte(volume_arduino_send));
            std::cout<<" "<<volume_arduino_send<<std::endl;
        }else{
            serialArduino[i].writeByte(Byte(value));
            std::cout<<" "<<value<<std::endl;
        }
    }
    //std::cout<<hue_first_send<<" "<<hue_second_send<<" "<<mode<<std::endl;
    ////////////////////////////////
    
    //GUI/////////////////////////////
    TParty.draw(980,25,200,200);//黒の時は180,150
    cloth.draw(cloth_image_positon.x,cloth_image_positon.y,cloth_image_size.x,cloth_image_size.y);
    gui.draw();//これはGUIの最後に
    //hue1
    ofColor c1;
    c1.setHsb(ofMap(hue_first,0,360,0,255), 255, 255);
    ofSetColor(c1);
    ofFill();
    ofRect(250,70,30,30);
    ofSetColor(255);
    ofNoFill();
    ofDrawBitmapString("hue_first",250,50);
    //hue1 end
    //hue2
    ofColor c2;
    c2.setHsb(ofMap(hue_second,0,360,0,255), 255, 255);
    ofSetColor(c2);
    ofFill();
    ofRect(350,70,30,30);
    ofSetColor(255);
    ofNoFill();
    ofDrawBitmapString("hue_second",350,50);
    //hue2 end
    //fft_hue
    ofColor c3;
    c3.setHsb(ofMap(send_fft_hue,0,100,0,255), 255, 255);
    ofSetColor(c3);
    ofFill();
    ofRect(300,130,30,30);
    ofSetColor(255);
    ofNoFill();
    ofDrawBitmapString("fft_hue",300,120);
    //fft_hue end
    
    //font
    if(using_volume==true){
      mic_on_or_off.drawString("mic->ON", 230, 220);
    }else{
        mic_on_or_off.drawString("mic->OFF", 230, 220);
    }
    
    if(using_fft_hue==true){
        fft_hue_on_or_off.drawString("fft_hue->ON", 230, 250);
    }else{
        fft_hue_on_or_off.drawString("fft_hue->OFF", 230, 250);
    }
    ///////////////////////////////////
    
    
    //余計だけど入れるところ/////////////
    string msg = ofToString((int) ofGetFrameRate()) + " fps";
    ofDrawBitmapString(msg, ofGetWidth() - 80, ofGetHeight() - 20);
    //////////////////////////////////
    
    //変数の0化
    buffer_sum=0;
    frequency_volume_max_volume=0;
    frequency_volume_max=0;
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    switch(key){
        case 'z':
            mode=201;
            break;
        case 'x':
            mode=202;
            break;
        case 'k':
            mode=203;
            break;
        case 'l':
            mode=204;
            break;
        case 'o':
            mode=205;
            break;
        case 'p':
            mode=206;
            break;
        case 'q':
            mode=207;
            break;
        case 'w':
            mode=208;
            break;
        case 'e':
            mode=209;
            break;
        case 'c':
            mode=210;
            break;
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::OSCrecv(){
    
    while( receiver.hasWaitingMessages() )
    {
        
        ofxOscMessage m;
        
        receiver.getNextMessage( &m );
        
        //OSC message setting
        if ( m.getAddress() == "/brightness" ){
            osc_in[0] = m.getArgAsFloat(0);
        }else if( m.getAddress() == "/hue1"){
            osc_in[1] = m.getArgAsFloat(0);
        }else if( m.getAddress() == "/hue2"){
            osc_in[2] = m.getArgAsFloat(0);
        }else if( m.getAddress() == "/sound_ratio_in"){
            osc_in[5] = m.getArgAsFloat(0);
        }else if(m.getAddress() == "/switch_hue1"){
            mode_switch[0] = m.getArgAsString(0);
        }else if(m.getAddress() == "/switch_hue2"){
            mode_switch[1] = m.getArgAsString(0);
        }else if(m.getAddress() == "/k"){
            mode_switch[2] = m.getArgAsString(0);
        }else if(m.getAddress() == "/l"){
            mode_switch[3] = m.getArgAsString(0);
        }else if(m.getAddress() == "/w"){
            mode_switch[4] = m.getArgAsString(0);
        }else if(m.getAddress() == "/e"){
            mode_switch[5] = m.getArgAsString(0);
        }else if(m.getAddress() == "/q"){
            mode_switch[6] = m.getArgAsString(0);
        }else if(m.getAddress() == "/bool_volume"){
            mode_switch[7] = m.getArgAsString(0);
        }else if(m.getAddress() == "/bool_fft_hue"){
            mode_switch[8] = m.getArgAsString(0);
        }
    }
}



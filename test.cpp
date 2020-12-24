/* 
rtmp://live-vie.twitch.tv/app/live_625326382_3ZbL8TCuQWNry5F0j4H33YNekRQRni

stream_key = live_625326382_3ZbL8TCuQWNry5F0j4H33YNekRQRni

gst-launch-1.0 rtmpsrc location=rtmp://34.105.124.140:1935/live/pri_test ! flvdemux name=mydemuxer flvmux name=mymuxer ! queue ! rtmpsink location=rtmp://live-vie.twitch.tv/app/live_625326382_3ZbL8TCuQWNry5F0j4H33YNekRQRni  mydemuxer.audio ! queue ! mymuxer.audio mydemuxer.video ! queue ! mymuxer.video
*/
#include <iostream>
#include <gst/gst.h>
#include <string>

int main(int arg, char *argv[]) {
    GstElement *pipeline = nullptr;
    GstBus *bus = nullptr;
    GstMessage *msg = nullptr;
    std::string pipelineStr;

	
    if(arg != 3)
    {
        std::cout << "Usage ./gstreamer_test [RTMP SOURCE ADDRESS] [RTMP SINK ADDRESS]" << std::endl;
        return 0;
    }

    std::string rtmpAddrIn = argv[1];
    std::string rtmpAddrOut = argv[2];

	pipelineStr = "rtmpsrc location=";
	pipelineStr += rtmpAddrIn;
	pipelineStr += " ! flvdemux name=mydemuxer flvmux name=mymuxer ! queue ! rtmpsink location=" ;
	pipelineStr += rtmpAddrOut;
	pipelineStr += "  mydemuxer.audio ! queue ! mymuxer.audio mydemuxer.video ! queue ! mymuxer.video" ;

    // gstreamer initialization
    gst_init(&arg, &argv);

    // building pipeline
    pipeline = gst_parse_launch(pipelineStr.c_str(), nullptr);

    // start playing
    gst_element_set_state(pipeline, GST_STATE_PLAYING);

    //wait until error or EOS ( End Of Stream )
    bus = gst_element_get_bus(pipeline);
    msg = gst_bus_timed_pop_filtered(bus, GST_CLOCK_TIME_NONE,
                                     static_cast<GstMessageType>(GST_MESSAGE_ERROR | GST_MESSAGE_EOS));

    // free memory
    if (msg != nullptr)
        gst_message_unref(msg);
    gst_object_unref(bus);
    gst_element_set_state(pipeline, GST_STATE_NULL);
    gst_object_unref(pipeline);

    return 0;
}

#include <iostream>
#include <stdio.h>
#include <fstream>
#include <time.h>

#include <seatrac_driver/SeatracDriver.h>
#include <seatrac_driver/messages/Messages.h>
#include <seatrac_driver/commands.h>
using namespace narval::seatrac;

int main(int argc, char *argv[])
{

    std::string test_name = "DummyTest";

    //TODO: either make these values editable by the command line or use #define 
    BID_E this_BID  = BEACON_ID_10;
    BID_E other_BID = BEACON_ID_1;
    float this_beacon_depth = 1; //TODO: units
    float other_beacon_depth = 1;
    float horizontal_distance = 1;
    int num_samples = 5;
    //TODO: add a header to the file with all this info in it.

    std::string serial_port;
    std::string out_file_path;
    switch(argc) {
        case 1: { serial_port = "/dev/ttyUSB0"; out_file_path = "./data/output.csv"; } break;
        case 2: { serial_port = argv[1]; out_file_path = "./data/output.csv"; } break;
        default: { serial_port = argv[1]; out_file_path = argv[2];} break;
    }

    std::ofstream output(out_file_path);
    SeatracDriver seatrac(serial_port);

    //command::set_beacon_id(seatrac, this_BID);

    output << "time, msg#, src_id, dest_id, flags, yaw, pitch, roll, local_depth, VOS, RSSI, range, azimuth, elevation, easting, northing, depth\n";

    for(int i=0; i<num_samples; i++) {
        command::data_send(
            seatrac, 
            other_BID, 
            MSG_REQU, 
            (uint8_t)4, 
            (uint8_t*)&i
        );
        messages::DataReceive resp;
        seatrac.wait_for_message(CID_DAT_RECEIVE, &resp);
        output << "TIME" << ", " 
               << i << ", " 
               << (uint8_t)resp.acoFix.srcId << ", "
               << (uint8_t)resp.acoFix.destId << ", "
               << resp.acoFix.flags << ", "
               << resp.acoFix.attitudeYaw << ", "
               << resp.acoFix.attitudePitch << ", "
               << resp.acoFix.attitudeRoll << ", "
               << resp.acoFix.depthLocal << ", "
               << resp.acoFix.vos << ", "
               << resp.acoFix.rssi << ", "
               //<< resp.acoFix.usbl.rssi << ", "
               << resp.acoFix.range.dist << ", "
               << resp.acoFix.usbl.azimuth << ", "
               << resp.acoFix.usbl.elevation << ", "
               << resp.acoFix.usbl.fitError << ", "
               << resp.acoFix.position.easting << ", "
               << resp.acoFix.position.northing << ", "
               << resp.acoFix.position.depth << "\n";

        //Comment this out if the other beacon won't be sending messages
        seatrac.wait_for_message(CID_DAT_RECEIVE, &resp);
    }
    
    output.close();

    return 0;
}

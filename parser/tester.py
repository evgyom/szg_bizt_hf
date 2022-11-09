from pathlib import Path
import os
from ctypes import *
import time

######
## Very imporant to use the correct buffer size. At least as big as the one used by the parser lib.
######
BUF_SIZE = 512

# c structure for the output data
class struct_out(Structure):
    _fields_ = [("n_frames", c_int32),  
                ("creator_name_length", c_int32),
                ("creator_name", c_char * BUF_SIZE),
                ("creation_year", c_int32),
                ("creation_month", c_int32),
                ("creation_day", c_int32),
                ("creation_hour", c_int32),
                ("creation_minute", c_int32),
                ("captions", c_char * BUF_SIZE),
                ("captions_length", c_int32),
                ("tags", c_char * BUF_SIZE),
                ("tags_length", c_int32),
                ("n_tags", c_int32),
                ("total_duration", c_int32),
                ("frame_height", c_int32),
                ("frame_width", c_int32)]


if __name__ == "__main__":
    project_root = Path.cwd()
    caff_file_path = project_root.joinpath("caff_examples").joinpath("1.caff")
    gif_out_path = project_root.joinpath("caff1.gif")
    lib_path = project_root.joinpath("build").joinpath("caff_parser").joinpath("libcaff_parser_shared.dll")

    # Load library
    caff_parser = CDLL(str(lib_path))

    # Define the input types
    caff_parser.parse.argtypes = [c_char_p, c_long, c_char_p, POINTER(struct_out)]

    # Call the function
    test_struct = struct_out(8,2)
    in_path = str(caff_file_path).encode("utf-8")
    out_path = str(gif_out_path).encode("utf-8")
    file_size = os.path.getsize(caff_file_path)

    retval = caff_parser.parse(in_path, file_size, out_path, byref(test_struct))
    
    print("CAFF processed with status:",retval)
    print("N_frames:", test_struct.n_frames)
    print("Creator name length:", test_struct.creator_name_length)
    print("Creator name:", test_struct.creator_name[0:test_struct.creator_name_length].decode("UTF-8"))
    print("Captions:", test_struct.captions[0:test_struct.captions_length].decode("UTF-8"))
    print("Number of tags:", test_struct.n_tags)
    print("Tags length:", test_struct.tags_length)

   #for i in range(test_struct.tags_length):
   #    print(test_struct.tags[i])
   #all_tags = test_struct.tags
   #print("Tags:", all_tags)
    print("Frame size: {} x {}".format(test_struct.frame_width, test_struct.frame_height))
    print("Total duration (ms):", test_struct.total_duration)

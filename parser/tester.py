from pathlib import Path
import os
from ctypes import *
import time

# c structure for the output data
class MyStruct(Structure):
    _fields_ = [("a", c_int),
                ("b", c_float),
                ("point_array",  * 4)]

if __name__ == "__main__":
    project_root = Path.cwd()
    caff_file_path = project_root.joinpath("caff_examples").joinpath("1.caff")
    gif_out_path = project_root.joinpath("caff1.gif")
    lib_path = project_root.joinpath("build").joinpath("caff_parser").joinpath("libcaff_parser_shared.dll")

    # Load library
    caff_parser = CDLL(str(lib_path))



    # Define the input types
    caff_parser.parse.argtypes = [c_char_p, c_long, c_char_p, POINTER(c_char)]

    # Call the function
    start_time = time.time()
    ######
    ## Very imporant to use the correct buffer size. At least as big as the one used by the parser lib.
    ######
    creator_name = (c_char*512)()
    retval = caff_parser.parse(str(caff_file_path).encode("utf-8"), os.path.getsize(caff_file_path), str(gif_out_path).encode("utf-8"), creator_name)
    end_time = time.time()
    print("GIF generated in:{} s".format(end_time-start_time))
    print(retval)
    print(creator_name[0:12].decode("utf-8"))
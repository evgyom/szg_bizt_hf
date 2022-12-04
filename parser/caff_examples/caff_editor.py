from pathlib import Path

caff_folder = Path.cwd()

def get_section_from_binary(path_in, path_out, start, end):
    with open(path_in, "rb") as f:
        # Not needed part
        f.read(start)
        # The needed part
        bytes_in = f.read(end - start)
    with open(path_out, "wb") as f:
        f.write(bytes_in)

if __name__ == "__main__":
    path_in = caff_folder.joinpath("1.caff")
    path_out = caff_folder.joinpath("1_header_only_1.caff")
    get_section_from_binary(path_in, path_out, 0, 29)

    path_out = caff_folder.joinpath("1_credits_only_1.caff")
    get_section_from_binary(path_in, path_out, 29, 64)

    path_out = caff_folder.joinpath("1_header_only_eof.caff")
    get_section_from_binary(path_in, path_out, 0, 28)

    path_out = caff_folder.joinpath("1_credits_only_eof.caff")
    get_section_from_binary(path_in, path_out, 29, 63)

    path_out = caff_folder.joinpath("1_ciff_only.caff")
    get_section_from_binary(path_in, path_out, 64, 2001172)

    path_in = caff_folder.joinpath("2.caff")
    path_out = caff_folder.joinpath("2_header_only.caff")
    get_section_from_binary(path_in, path_out, 0, 29)
    path_out = caff_folder.joinpath("2_credits_only.caff")
    get_section_from_binary(path_in, path_out, 29, 66)
    path_out = caff_folder.joinpath("2_ciff_only.caff")
    get_section_from_binary(path_in, path_out, 66, 2001173)

    path_in = caff_folder.joinpath("3.caff")
    path_out = caff_folder.joinpath("3_header_only.caff")
    get_section_from_binary(path_in, path_out, 0, 29)
    path_out = caff_folder.joinpath("3_credits_only.caff")
    get_section_from_binary(path_in, path_out, 29, 52)
    path_out = caff_folder.joinpath("3_ciff_only.caff")
    get_section_from_binary(path_in, path_out, 52, 2001160)

    path_in = caff_folder.joinpath("1.caff")
    path_out = caff_folder.joinpath("fuzzer.caff")
    get_section_from_binary(path_in, path_out, 0, 1000000)
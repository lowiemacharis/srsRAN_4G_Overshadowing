
execute_process(
COMMAND git rev-parse --abbrev-ref HEAD
WORKING_DIRECTORY "/Users/lowie/Documents/FHSTP/Academiejaar2024-2025/S2/Internship_MasterThesis/srsRAN_4G_Overshadowing"
OUTPUT_VARIABLE GIT_BRANCH
OUTPUT_STRIP_TRAILING_WHITESPACE
)

execute_process(
COMMAND git log -1 --format=%h
WORKING_DIRECTORY "/Users/lowie/Documents/FHSTP/Academiejaar2024-2025/S2/Internship_MasterThesis/srsRAN_4G_Overshadowing"
OUTPUT_VARIABLE GIT_COMMIT_HASH
OUTPUT_STRIP_TRAILING_WHITESPACE
)

message(STATUS "Generating build_info.h")
configure_file(
  /Users/lowie/Documents/FHSTP/Academiejaar2024-2025/S2/Internship_MasterThesis/srsRAN_4G_Overshadowing/lib/include/srsran/build_info.h.in
  /Users/lowie/Documents/FHSTP/Academiejaar2024-2025/S2/Internship_MasterThesis/srsRAN_4G_Overshadowing/cmake-build-debug/lib/include/srsran/build_info.h
)

/**
 *
 * \section COPYRIGHT
 *
 * Copyright 2013-2020 Software Radio Systems Limited
 *
 * By using this file, you agree to the terms and conditions set
 * forth in the LICENSE file which can be found at the top level of
 * the distribution.
 *
 */

#ifndef SRSLTE_UCI_NR_H
#define SRSLTE_UCI_NR_H

#include "srslte/phy/common/phy_common_nr.h"
#include "srslte/phy/fec/crc.h"
#include "srslte/phy/fec/polar/polar_code.h"
#include "srslte/phy/fec/polar/polar_decoder.h"
#include "srslte/phy/fec/polar/polar_encoder.h"
#include "srslte/phy/fec/polar/polar_rm.h"
#include "srslte/phy/phch/phch_cfg_nr.h"
#include "srslte/phy/phch/pucch_cfg_nr.h"
#include "uci_cfg_nr.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief NR-UCI Encoder/decoder initialization arguments
 */
typedef struct {
  bool  disable_simd;         ///< Disable Polar code SIMD
  float block_code_threshold; ///< Set normalised block code threshold (receiver only)
  float one_bit_threshold;    ///< Decode threshold for 1 bit (receiver only)
} srslte_uci_nr_args_t;

typedef struct {
  srslte_carrier_nr_t    carrier;
  srslte_polar_rm_t      rm_tx;
  srslte_polar_rm_t      rm_rx;
  srslte_polar_encoder_t encoder;
  srslte_polar_decoder_t decoder;
  srslte_crc_t           crc6;
  srslte_crc_t           crc11;
  srslte_polar_code_t    code;
  uint8_t*               bit_sequence;         ///< UCI bit sequence
  uint8_t*               c;                    ///< UCI code-block prior encoding or after decoding
  uint8_t*               allocated;            ///< Polar code intermediate
  uint8_t*               d;                    ///< Polar code encoded intermediate
  float                  block_code_threshold; ///< Decode threshold for block code (3-11 bits)
  float                  one_bit_threshold;    ///< Decode threshold for 1 bit
} srslte_uci_nr_t;

/**
 * @brief Calculates the number of bits carried by PUCCH formats 2, 3 and 4 from the PUCCH resource
 * @remark Defined in TS 38.212 Table 6.3.1.4-1: Total rate matching output sequence length Etot
 * @param resource PUCCH format 2, 3 or 4 Resource provided by upper layers
 * @return The number of bits if the provided resource is valid, SRSLTE_ERROR code otherwise
 */
SRSLTE_API int srslte_uci_nr_pucch_format_2_3_4_E(const srslte_pucch_nr_resource_t* resource);

/**
 * @brief Calculates in advance how many CRC bits will be appended for a given amount of UCI bits (A)
 * @remark Defined in TS 38.212 section 6.3.1.2 Code block segmentation and CRC attachment
 * @param A Number of UCI bits to transmit
 */
SRSLTE_API uint32_t srslte_uci_nr_crc_len(uint32_t A);

/**
 * @brief Initialises NR-UCI encoder/decoder object
 * @param[in,out] q NR-UCI object
 * @param[in] args Configuration arguments
 * @return SRSLTE_SUCCESS if initialization is successful, SRSLTE_ERROR code otherwise
 */
SRSLTE_API int srslte_uci_nr_init(srslte_uci_nr_t* q, const srslte_uci_nr_args_t* args);

/**
 * @brief Sets NR carrier
 * @param[in,out] q NR-UCI object
 * @param carrier Provides carrier configuration
 * @return SRSLTE_SUCCESS if successful, SRSLTE_ERROR code otherwise
 */
SRSLTE_API int srslte_uci_nr_set_carrier(srslte_uci_nr_t* q, const srslte_carrier_nr_t* carrier);

/**
 * @brief Deallocates NR-UCI encoder/decoder object
 * @param[in,out] q NR-UCI object
 */
SRSLTE_API void srslte_uci_nr_free(srslte_uci_nr_t* q);

/**
 * @brief Encodes UCI bits
 *
 * @attention Compatible only with PUCCH formats 2, 3 and 4
 *
 * @remark Defined in TS 38.212 section 6.3.1.1
 *
 * @param[in,out] q NR-UCI object
 * @param[in] pucch_cfg Higher layers PUCCH configuration
 * @param[in] uci_cfg UCI configuration
 * @param[in] uci_value UCI values
 * @param[out] o Output encoded bits
 * @return Number of encoded bits if encoding is successful, SRSLTE_ERROR code otherwise
 */
SRSLTE_API int srslte_uci_nr_encode_pucch(srslte_uci_nr_t*                  q,
                                          const srslte_pucch_nr_resource_t* pucch_resource,
                                          const srslte_uci_cfg_nr_t*        uci_cfg,
                                          const srslte_uci_value_nr_t*      value,
                                          uint8_t*                          o);

/**
 * @brief Decoder UCI bits
 *
 * @attention Compatible only with PUCCH formats 2, 3 and 4
 *
 * @param[in,out] q NR-UCI object
 * @param[in] pucch_resource_cfg
 * @param[in] uci_cfg
 * @param[in] llr
 * @param[out] value
 * @return SRSLTE_SUCCESSFUL if it is successful, SRSLTE_ERROR code otherwise
 */
SRSLTE_API int srslte_uci_nr_decode_pucch(srslte_uci_nr_t*                  q,
                                          const srslte_pucch_nr_resource_t* pucch_resource_cfg,
                                          const srslte_uci_cfg_nr_t*        uci_cfg,
                                          int8_t*                           llr,
                                          srslte_uci_value_nr_t*            value);

/**
 * @brief Calculates total number of encoded bits for HARQ-ACK multiplexing in PUSCH
 * @param[in] cfg PUSCH transmission configuration
 * @return The number of encoded bits if successful, SRSLTE_ERROR code otherwise
 */
SRSLTE_API int srslte_uci_nr_pusch_ack_nof_bits(const srslte_uci_nr_pusch_cfg_t* cfg, uint32_t O_ack);

/**
 * @brief Encodes HARQ-ACK bits for PUSCH transmission
 * @param[in,out] q NR-UCI object
 * @param[in] cfg UCI configuration
 * @param[in] value UCI value
 * @param[out] o_ack Encoded ack bits
 * @return The number of encoded bits if successful, SRSLTE_ERROR code otherwise
 */
SRSLTE_API int srslte_uci_nr_encode_pusch_ack(srslte_uci_nr_t*             q,
                                              const srslte_uci_cfg_nr_t*   cfg,
                                              const srslte_uci_value_nr_t* value,
                                              uint8_t*                     o_ack);

/**
 * @brief Decodes HARQ-ACK bits for PUSCH transmission
 * @param[in,out] q NR-UCI object
 * @param[in] cfg UCI configuration
 * @param[in] llr Provides softbits LLR
 * @param[out] value UCI value
 * @return SRSLTE_SUCCESS if the decoding process was successful, SRSLTE_ERROR code otherwise
 */
SRSLTE_API int srslte_uci_nr_decode_pusch_ack(srslte_uci_nr_t*           q,
                                              const srslte_uci_cfg_nr_t* cfg,
                                              int8_t*                    llr,
                                              srslte_uci_value_nr_t*     value);

/**
 * @brief Calculates total number of encoded bits for CSI part 1 multiplexing in PUSCH
 * @param[in] cfg UCI configuration
 * @return The number of encoded bits if valid, SRSLTE_ERROR code otherwise
 */
SRSLTE_API int srslte_uci_nr_pusch_csi1_nof_bits(const srslte_uci_cfg_nr_t* cfg);

/**
 * @brief Encodes CSI part 1 bits for PUSCH transmission
 * @param[in,out] q NR-UCI object
 * @param[in] cfg UCI configuration
 * @param[in] value UCI value
 * @param[out] o_ack Encoded CSI part 1 bits
 * @return The number of encoded bits if successful, SRSLTE_ERROR code otherwise
 */
SRSLTE_API int srslte_uci_nr_encode_pusch_csi1(srslte_uci_nr_t*             q,
                                               const srslte_uci_cfg_nr_t*   cfg,
                                               const srslte_uci_value_nr_t* value,
                                               uint8_t*                     o);

/**
 * @brief Decodes CSI part 1 bits for PUSCH transmission
 * @param[in,out] q NR-UCI object
 * @param[in] cfg UCI configuration
 * @param[in] llr Provides softbits LLR
 * @param[out] value UCI value
 * @return SRSLTE_SUCCESS if the decoding process was successful, SRSLTE_ERROR code otherwise
 */
SRSLTE_API int srslte_uci_nr_decode_pusch_csi1(srslte_uci_nr_t*           q,
                                               const srslte_uci_cfg_nr_t* cfg,
                                               int8_t*                    llr,
                                               srslte_uci_value_nr_t*     value);

/**
 * @brief Calculates the total number of UCI bits
 * @param uci_cfg UCI configuration
 * @return The number of total bits carrying HARQ ACK, CSI reports and SR bits
 */
SRSLTE_API uint32_t srslte_uci_nr_total_bits(const srslte_uci_cfg_nr_t* uci_cfg);

/**
 * @brief Converts to string an UCI data structure
 * @param uci_data UCO data structure
 * @param str Destination string
 * @param str_len String length
 * @return Resultant string length
 */
SRSLTE_API uint32_t srslte_uci_nr_info(const srslte_uci_data_nr_t* uci_data, char* str, uint32_t str_len);

#endif // SRSLTE_UCI_NR_H

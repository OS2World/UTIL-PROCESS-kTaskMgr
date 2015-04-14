/* $Id: kTaskMgr_Defs.h,v 1.2 2000/06/19 20:08:42 stknut Exp $
 *
 * Common resource defines for kTaskMgr.
 *
 * Copyright (c) 2000 knut st. osmundsen
 *
 */

#define KTASKMGR                        1


/**************/
/* DIALOG IDs */
/**************/
#define DL_KTASKMGR                     1000
#define KTM_NTBK                        1001

#define DL_KPROCESSPAGE                 1100
#define KPROCPG_CNR                     1101

#define DL_KMODULEPAGE                  1200
#define KMODPG_CNR                      1201

#define DL_KMODULEDETAILS               1220
#define KMODDTL_TXT_HMTE                1221
#define KMODDTL_TXT_HMTE_LBL            1222
#define KMODDTL_TXT_MODEL               1223
#define KMODDTL_TXT_MODEL_LBL           1224
#define KMODDTL_TXT_NAME                1225
#define KMODDTL_TXT_NAME_LBL            1226
#define KMODDTL_TXT_COBJECTS            1227
#define KMODDTL_TXT_COBJECTS_LBL        1228
#define KMODDTL_CNR_OBJECTS             1229
#define KMODDTL_TXT_CIMPMODULES         1230
#define KMODDTL_TXT_CIMPMODULES_LBL     1231
#define KMODDTL_CNR_IMPMODULES          1232

#define DL_KABOUTPAGE                   1300
#define KABOUTPG_PB_BELIEVERS           1301

#define DL_KPROCESSDETAILS              1400
#define KPROCDTL_NTBK                   1401

#define DL_KPROCESS_CHANGE_PRIORITY     1450
#define KPROCPRTY_CB_CLASS              1451
#define KPROCPRTY_TXT_CLASS_LBL         1452
#define KPROCPRTY_SPB_DELTA             1453
#define KPROCPRTY_TXT_DELTA_LBL         1454
#define KPROCPRTY_CNR_THREADS           1455

#define DL_KPROCESSDETAILSPAGE          1500
#define KPROCDTLPG_TXT_PID              1501
#define KPROCDTLPG_TXT_PID_LBL          1502
#define KPROCDTLPG_TXT_PPID             1503
#define KPROCDTLPG_TXT_PPID_LBL         1504
#define KPROCDTLPG_TXT_HMTE             1505
#define KPROCDTLPG_TXT_HMTE_LBL         1506
#define KPROCDTLPG_TXT_TYPE             1507
#define KPROCDTLPG_TXT_TYPE_LBL         1508
#define KPROCDTLPG_TXT_STATUS           1509
#define KPROCDTLPG_TXT_STATUS_LBL       1510
#define KPROCDTLPG_TXT_CTHREADS         1511
#define KPROCDTLPG_TXT_CTHREADS_LBL     1512
#define KPROCDTLPG_TXT_C32SEMAPHORS     1513
#define KPROCDTLPG_TXT_C32SEMAPHORS_LBL 1514
#define KPROCDTLPG_TXT_CSHAREDMEM       1515
#define KPROCDTLPG_TXT_CSHAREDMEM_LBL   1516
#define KPROCDTLPG_TXT_CFILES           1517
#define KPROCDTLPG_TXT_CFILES_LBL       1518
#define KPROCDTLPG_TXT_CDLLS            1519
#define KPROCDTLPG_TXT_CDLLS_LBL        1520
#define KPROCDTLPG_TXT_SCREENGRPID      1521
#define KPROCDTLPG_TXT_SCREENGRPID_LBL  1522
#define KPROCDTLPG_TXT_CCHILDREN        1523
#define KPROCDTLPG_TXT_CCHILDREN_LBL    1524
#define KPROCDTLPG_TXT_NAME             1525
#define KPROCDTLPG_TXT_NAME_LBL         1526
#define KPROCDTLPG_CNR_THREADS          1527
#define KPROCDTLPG_CNR_CHILDREN         1528

#define DL_KSFNPAGE                     1600
#define KSFNPG_CNR                      1601

#define DL_KQUEUEPAGE                   1700
#define KQUEPG_CNR                      1701

#define DL_KQUEUEOPENPAGE               1800
#define KQUEOPG_CNR                     1801

#define DL_KCONTAINERPAGE               3000
#define KCNRPG_CNR                      3001

#define DL_ABOUT                        3200

#define DL_KBELIEVER                    3300
#define DL_KBELIEVER_MLE                3301

#define DL_KSFNDETAILS                  3400
#define KSFNDTL_NTBK                    3401



/************/
/* MENU IDs */
/************/
#define IDM_CNR_ALL_REFRESH              9000
#define IDM_CNR_ALL_SORT                 9100
#define IDM_CNR_ALL_SORT_FIRST           9101
#define IDM_CNR_ALL_SORT_LAST            9142

#define IDM_HELP                        10000
#define IDM_ABOUT                       10001

#define IDM_CNR_MODULE                  10100
#define IDM_CNR_MOD_DETAILS             10101

#define IDM_CNR_MODULE_ALL              10200
#define IDM_CNR_MOD_ALL_REFRESH         IDM_CNR_ALL_REFRESH
#define IDM_CNR_MOD_ALL_SORT            IDM_CNR_ALL_SORT

#define IDM_CNR_PROCESS                 10300
#define IDM_CNR_PROC_DETAILS            10301
#define IDM_CNR_PROC_KILL               10302
#define IDM_CNR_PROC_KILLXF86           10303
#define IDM_CNR_PROC_PRIORITY           10304

#define IDM_CNR_PROCESS_ALL             10400
#define IDM_CNR_PROC_ALL_REFRESH        IDM_CNR_ALL_REFRESH
#define IDM_CNR_PROC_ALL_SORT           IDM_CNR_ALL_SORT

#define IDM_CNR_THREAD                  10500
#define IDM_CNR_TRD_DETAILS             10501

#define IDM_CNR_THREAD_ALL              10600
#define IDM_CNR_TRD_ALL_REFRESH         IDM_CNR_ALL_REFRESH
#define IDM_CNR_TRD_ALL_SORT            IDM_CNR_ALL_SORT

#define IDM_CNR_SFN                     10700
#define IDM_CNR_SFN_DETAILS             10701

#define IDM_CNR_SFN_ALL                 10750
#define IDM_CNR_SFN_ALL_REFRESH         IDM_CNR_ALL_REFRESH
#define IDM_CNR_SFN_ALL_SORT            IDM_CNR_ALL_SORT

#define IDM_CNR_QUEUE                   10800
#define IDM_CNR_QUE_OWNERDETAILS        10801

#define IDM_CNR_QUEUE_ALL               10900
#define IDM_CNR_QUE_ALL_REFRESH         IDM_CNR_ALL_REFRESH
#define IDM_CNR_QUE_ALL_SORT            IDM_CNR_ALL_SORT


#define IDM_CNR_QUEUEOPEN               11000
#define IDM_CNR_QUEO_PROCESSDETAILS     11001
#define IDM_CNR_QUEO_OWNERDETAILS       11002

#define IDM_CNR_QUEUEOPEN_ALL           11100
#define IDM_CNR_QUEO_ALL_REFRESH        IDM_CNR_ALL_REFRESH
#define IDM_CNR_QUEO_ALL_SORT           IDM_CNR_ALL_SORT

#define IDM_CNR_OBJECT_ALL              11200
#define IDM_CNR_OBJ_ALL_REFRESH         IDM_CNR_ALL_REFRESH
#define IDM_CNR_OBJ_ALL_SORT            IDM_CNR_ALL_SORT

#define IDM_CNR_PROCESS_DETAILS         11300
#define IDM_CNR_OBJ_PROC_DTL_PPID       11301
#define IDM_CNR_OBJ_PROC_DTL_HMTE       11302

#define IDM_CNR_PROCESS_DETAILS_ALL     11400
#define IDM_CNR_OBJ_ALL_REFRESH         IDM_CNR_ALL_REFRESH
#define IDM_CNR_OBJ_ALL_SORT            IDM_CNR_ALL_SORT

#define IDM_CNR_MEMMAP                  11500
#define IDM_CNR_MMAP_DETAILS            11501

#define IDM_CNR_MEMMAP_ALL              11550
#define IDM_CNR_MMAP_ALL_REFRESH        IDM_CNR_ALL_REFRESH
#define IDM_CNR_MMAP_ALL_SORT           IDM_CNR_ALL_SORT


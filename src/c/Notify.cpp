/*
	This file is part of Task-Aware GASPI and is licensed under the terms contained in the COPYING and COPYING.LESSER files.

	Copyright (C) 2018-2020 Barcelona Supercomputing Center (BSC)
*/

#include <GASPI.h>
#include <GASPI_Lowlevel.h>

#include "common/Environment.hpp"
#include "common/TaskingModel.hpp"

#include <cassert>

#ifdef __cplusplus
extern "C" {
#endif

gaspi_return_t
tagaspi_notify(const gaspi_segment_id_t segment_id_remote,
		const gaspi_rank_t rank,
		const gaspi_notification_id_t notification_id,
		const gaspi_notification_t notification_value,
		const gaspi_queue_id_t queue)
{
	assert(_env.enabled);
	gaspi_return_t eret;

	void *counter = TaskingModel::getCurrentEventCounter();
	assert(counter != NULL);

	gaspi_tag_t tag = (gaspi_tag_t) counter;

	// TODO: Use gaspi_operation_get_num_requests
	gaspi_number_t numRequests = 1;
	TaskingModel::increaseCurrentTaskEventCounter(counter, numRequests);

	eret = gaspi_operation_submit(GASPI_OP_NOTIFY, tag,
				0, 0, rank, segment_id_remote, 0, 0,
				notification_id, notification_value,
				queue, GASPI_BLOCK);
	assert(eret != GASPI_TIMEOUT);

	if (eret != GASPI_SUCCESS) {
		TaskingModel::decreaseTaskEventCounter(counter, numRequests);
	}

	return eret;
}

#ifdef __cplusplus
}
#endif


/**
 *Licensed to the Apache Software Foundation (ASF) under one
 *or more contributor license agreements.  See the NOTICE file
 *distributed with this work for additional information
 *regarding copyright ownership.  The ASF licenses this file
 *to you under the Apache License, Version 2.0 (the
 *"License"); you may not use this file except in compliance
 *with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *Unless required by applicable law or agreed to in writing,
 *software distributed under the License is distributed on an
 *"AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 *specific language governing permissions and limitations
 *under the License.
 */
/*
 * activator.c
 *
 *  Created on: Aug 20, 2010
 *      Author: alexanderb
 */
#include <stdlib.h>
#include <stdio.h>
#include <apr_general.h>

#include "bundle_activator.h"
#include "bundle_context.h"
#include "mongoose.h"

struct userData {
	struct mg_context *ctx;
};

celix_status_t bundleActivator_create(BUNDLE_CONTEXT context, void **userData) {
	apr_pool_t *pool;
	celix_status_t status = bundleContext_getMemoryPool(context, &pool);
	*userData = apr_palloc(pool, sizeof(struct userData));
	return CELIX_SUCCESS;
}

celix_status_t bundleActivator_start(void * userData, BUNDLE_CONTEXT context) {
	BUNDLE bundle;
	celix_status_t status = CELIX_SUCCESS;
	struct userData * data = (struct userData *) userData;

	if (bundleContext_getBundle(context, &bundle) == CELIX_SUCCESS) {
		char *entry = NULL;
		bundle_getEntry(bundle, "root", &entry);

		const char *options[] = {
			"document_root", entry,
			NULL
		};
		data->ctx = mg_start(NULL, options);

		printf("Mongoose started on: %s\n", mg_get_option(data->ctx, "listening_ports"));
	} else {
		status = CELIX_START_ERROR;
	}

	return status;
}

celix_status_t bundleActivator_stop(void * userData, BUNDLE_CONTEXT context) {
	struct userData * data = (struct userData *) userData;
	mg_stop(data->ctx);
	printf("Mongoose stopped\n");
	return CELIX_SUCCESS;
}

celix_status_t bundleActivator_destroy(void * userData, BUNDLE_CONTEXT context) {
	return CELIX_SUCCESS;
}

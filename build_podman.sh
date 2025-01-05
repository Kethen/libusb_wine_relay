set -xe
IMAGE_TAG="libusb_wine_relay_build"

if [ -n "$REBUILD_IMAGE" ]
then
	podman image rm -f $IMAGE_TAG
	podman image rm -f ${IMAGE_TAG}_aarch64
fi

if ! podman image exists $IMAGE_TAG
then
	podman image build -t $IMAGE_TAG -f Dockerfile --arch=x86_64
fi

if ! podman image exists ${IMAGE_TAG}_aarch64
then
	podman image build -t ${IMAGE_TAG}_aarch64 -f Dockerfile --arch=arm64
fi

podman run \
	--rm -it \
	--security-opt label=disable \
	-v ./:/workdir \
	-v ./build.sh:/workdir/build.sh:ro \
	-w /workdir \
	--entrypoint /bin/bash \
	$IMAGE_TAG \
	build.sh

podman run \
	--rm -it \
	--security-opt label=disable \
	-v ./:/workdir \
	-v ./build.sh:/workdir/build.sh:ro \
	-w /workdir \
	--entrypoint /bin/bash \
	${IMAGE_TAG}_aarch64 \
	build_aarch64.sh

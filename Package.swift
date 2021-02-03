// swift-tools-version:5.3

import PackageDescription
import Foundation

let package = Package(
    name: "GMI",
    platforms: [
        .iOS(.v13)
    ],
    products: [
        .library(
            name: "IWSAuthSDKiOS",
            targets: ["IWSAuthSDKiOS"])
        
    ],
    dependencies: [
        .package(name: "Realm", url: "https://github.com/realm/realm-cocoa", .exact(Version("10.5.0")))
    ],
    targets: [
        .binaryTarget(name: "GMI", url: "https://github.com/ImageWare/IWSAuthSDKiOS/raw/main/GMI.xcframework.zip", checksum: "70f9bb99a374d2ad74ebc6f949f93b14d4ef40769c73ed9dbed178e9abb11c32"),
        .binaryTarget(name: "IWS_Shared_Components",
                      path: "IWS_Shared_Components.xcframework"),
        
        .binaryTarget(name: "PalmAPI", path: "PalmAPI.xcframework"),
        .target(name: "GMIPackage",
                        dependencies: [
                            .target(name: "GMI", condition: .when(platforms: .some([.iOS]))),
                            .target(name: "IWS_Shared_Components", condition: .when(platforms: .some([.iOS]))),
                            .target(name: "PalmAPI", condition: .when(platforms: .some([.iOS]))),
                            .product(name: "RealmSwift", package: "Realm"),
                            .product(name: "Realm", package: "Realm")
                        ],
                        path: "GMIPath"
                )
    ],
    swiftLanguageVersions: [.v5]
)
